#include "game.h"
#include "assistant.h"
#include <thread>
#include <random>

const std::string stateStr[] = {"preflop", "flop", "turn", "river", "end"};


template <class ElemType >
inline void Swap(ElemType &e1, ElemType &e2)
// 操作结果: 交换e1, e2之值
{
	ElemType temp;		// 临时变量
	// 循环赋值实现交换e1, e2
	temp = e1;	e1 = e2;  e2 = temp;
}

void Game::init_game() {
    chips = new int*[playerNum];
    for (int i = 0; i < playerNum; i++)
        chips[i] = new int[4]{0};

    ftag = new bool[playerNum];
    for (int i = 0; i < playerNum; i++)
        ftag[i] = false;

    ctag = new bool[playerNum];
    for (int i = 0; i < playerNum; i++)
        ctag[i] = false;

    active = (dealer + 3) % playerNum;
}

void Game::init_players(const HumanPlayer& p, const int& c) {
    for (int i = 1; i < playerNum; i++)
        players.push_back(std::make_unique<BotPlayer>("BotPlayer"+std::to_string(i), c));
    players.insert(players.begin() + hpi, std::make_unique<HumanPlayer>(p));
    // commit blinds
    int sb = pos.find(" S B ");
    int bb = pos.find(" B B ");
    chips[sb][0] = 1;
    chips[bb][0] = 2;
    players[sb]->setChips(c - 1);
    players[bb]->setChips(c - 2);
    lastBet = bb;
}

void Game::checkState() {
    int i;
    for (i = 0; i < playerNum; i++)
    {
        if (ftag[i]) continue;
        if (!ctag[i]) break;
    }
    if (i == playerNum) {   // 进入下一阶段
        stateCode++;
        lastBet = -1;   // 清空lasetBet指针
        for (int i = 0; i < playerNum; i++) // 清空check tags
            if (!ftag[i])
                ctag[i] = false;
        // 移动active指针到庄位后一位
        active = dealer;
        step();
    }
}


std::vector<Card> Game::getHands(const int& k) const {
    std::vector<Card> temp;
    if (stateCode) {
        const auto& pub = deck_.getPubCards();
        temp.assign(pub.begin(), pub.end() - 3 + stateCode);
    }
    temp.insert(temp.end(), hands[k].begin(), hands[k].end());
    return temp;
}

std::vector<Card> Game::getFinalHands(const int& k) const {
    std::vector<Card> temp;
    const auto& pub = deck_.getPubCards();
    temp.assign(pub.begin(), pub.end());
    temp.insert(temp.end(), hands[k].begin(), hands[k].end());
    return temp;
}

std::vector<double> Game::calcWinRate(const int& simulations) const {
    std::vector<double> win(playerNum, 0.0);
    if (stateCode < 3) {
        int known_pub_cards_num = (stateCode) ? stateCode + 2 : 0;
        std::vector<Card> deck_remain = deck_.remainingDeck(playerNum, known_pub_cards_num);

        int num_threads = std::thread::hardware_concurrency();
        if (num_threads == 0) num_threads = 4;
        int per_thread = simulations / num_threads;

        std::vector<std::vector<double>> local_win(num_threads, std::vector<double>(playerNum, 0.0));
        std::vector<std::thread> threads;

        for (int t = 0; t < num_threads; t++) {
            threads.emplace_back([&, t]() {
                std::mt19937 engin(std::random_device{}());
                int start = t * per_thread;
                int end = (t == num_threads - 1) ? simulations : start + per_thread;

                for (int i = start; i < end; i++) {
                    auto tmp = deck_remain;
                    std::shuffle(tmp.begin(), tmp.end(), engin);
                    std::vector<Card> board(tmp.begin(), tmp.begin() + 5 - known_pub_cards_num);
                    const auto& pub = deck_.getPubCards();
                    board.insert(board.end(), pub.begin(), pub.begin() + known_pub_cards_num);
                    auto winners = checkWinner(board);
                    double share = 1.0 / winners.size();
                    for (auto j : winners)
                        local_win[t][j] += share;
                }
            });
        }

        for (auto& t : threads) t.join();

        for (int t = 0; t < num_threads; t++)
            for (int i = 0; i < playerNum; i++)
                win[i] += local_win[t][i];
    }

    if (stateCode >= 3) {
        auto winners = checkWinner();
        int n = winners.size();
        if (n == 1) win[winners[0]] = 1.0 * simulations;
    }

    for (int i = 0; i < playerNum; i++)
        win[i] = 100.0 * win[i] / simulations;
    return win;
}

std::vector<int> Game::checkWinner(std::vector<Card> public_cards) const {
    std::vector<int> res;
    int bestRank = INT_MAX;
    for (int i = 0; i < playerNum; i++) {
        if (!ftag[i]) {
            std::vector<Card> handCards = hands[i];
            handCards.insert(handCards.end(), public_cards.begin(), public_cards.end());
            int rank = advancedEvaluate(handCards);
            if (rank >= 0 && rank < bestRank) {
                res.clear();
                bestRank = rank;
                res.push_back(i);
            } else if (rank >= 0 && rank == bestRank) {
                res.push_back(i);
            }
        }
    }
    return res;
}

Game::Game(int pn, int d): playerNum(pn), dealer(d), stateCode(0) {
    init_game();
    deck_.shuffle();
    deck_.deal(playerNum, hands);
}

/**
 * @brief 使用此构造函数构建一个单人游戏
 * 
 * @param p (Position class) - Position类，包括玩家数量、位置信息
 * @param c (initialChips) - 初始筹码量，每位玩家的起始资金
 * @param hp (humanPlayer) - 人类玩家对象引用，包含姓名、当前手牌等信息
 * @param hppi (hp position index) - 人类玩家在桌面的位置标识
 */
Game::Game(const Position& p,const int& c, const HumanPlayer& hp, const int& hppi)
: playerNum(p.getPlayerNum()), dealer(p.getDealer()), stateCode(0) {
    pos = p;
    init_game();
    hpi = hppi;
    init_players(hp, c);
    deck_.shuffle();
    deck_.deal(playerNum, hands);
}

Game::~Game() {
    for (int i = 0; i < playerNum; i++)
        delete[] chips[i];
    delete[] chips;
    delete[] ftag;
    delete[] ctag;
}


void Game::show() const {
    std::cout << "================================================================" << std::endl;
    std::cout << "  Public: " << std::endl;
    std::cout << "\t\t\t" << deck_.pubCardsStr(stateCode) << std::endl;
    std::cout << "   State:  " << stateStr[stateCode] << std::endl;
    std::cout << "     Pot:  " << getPot() << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;
    auto win_rate = calcWinRate(20000);
    for (int i = 0; i < playerNum; i++) {
        if (i == active) std::cout << " *";
        else std::cout << "  ";
        std::cout << std::left << std::setw(12) << players[i]->getName();
        std::cout << " (" << pos[i] << "):   ";
        for (int j = 0; j < 2; j++)
            std::cout << hands[i][j].toString() << ' ';
        std::cout << "\t" << chips[i][stateCode];
        if (ftag[i])
            std::cout << "\t(fold)\t\t" << HandType::evaluate(getHands(i));
        else
            std::cout << "\t" << "胜率: " << std::fixed << std::setprecision(2) << win_rate[i] << "%\t" << HandType::evaluate(getHands(i));
        std::cout << std::endl;
    }
    std::cout << "================================================================" << std::endl;
}

void Game::showPlayerView() const {
    std::cout << "================================================================\n";
    std::cout << "  Public: \n";
    std::cout << "\t\t" << deck_.pubCardsColStr(stateCode) << "\n";
    std::cout << "   State:  " << stateStr[stateCode] << "\n";
    std::cout << "     Pot:  " << getPot() << "\n";
    std::cout << "----------------------------------------------------------------\n";

    for (int i = 0; i < playerNum; i++) {
        // active标记
        std::cout << (i == active ? " *" : "  ");
        //玩家名：固定宽度
        std::cout << std::left << std::setw(12) << players[i]->getName();
        std::cout << " (" << pos[i] << "):   ";
        // 手牌
        if (i == hpi) {
            for (int j = 0; j < 2; j++)
                std::cout << hands[i][j] << ' ';
        } else {
            std::cout << "[??] [??] ";
        }

        // 筹码
        std::cout << std::right << std::setw(5) << chips[i][stateCode] << " BB\n";
    }

    std::cout << "================================================================" << std::endl;
}

int Game::getPot() const {
    int temp = 0;
    for (int i = 0; i < playerNum; i++)
        for (int j = 0; j <= stateCode; j++)
            temp += chips[i][j];
    return temp;
}

void Game::fold() {
    if (hpi == active) {    // 唯一的人类玩家选择弃牌，游戏结束
        std::cout << "You folded. Better luck next time!" << std::endl;
        stateCode = 4;
    }
    ftag[active] = 1;
    int num = 0;
    for (int i = 0; i < playerNum; i++)
        if (!ftag[i]) num++;
    if (num <= 1) {stateCode = 4; return;}
    
    step();
    checkState();
}

void Game::call() {
    chips[active][stateCode] = commit[stateCode];
    ctag[active] = true;
    step();
    checkState();
}

void Game::bet(const int& chip) {
    if (chips[active][stateCode] + chip <= commit[stateCode])
        throw Error(1, "Invalid betting scale.");
    for (int i = 0; i < playerNum; i++)
        ctag[i] = false;    // 加注将清空其他人的check tag
    ctag[active] = true;    // 加注将自己标记成为check tag
    chips[active][stateCode] += chip;
    commit[stateCode] = chips[active][stateCode];
    lastBet = active;
    step();
}

void Game::toAct() { // 玩家筹码修改在Player的makeAction中处理
    int betAmount = 0;
    ACTION action = players[active]->makeAction(getChipsToCall(), betAmount);
    if (action == FOLD) {
        fold();
    } else if (action == CHECK || action == CALL) {
        call();
    } else if (action == BET || action == RAISE || action == ALLIN) {
        bet(betAmount);
    }
}

std::vector<int> Game::checkWinner() const {
    std::vector<int> res;
    int bestRank = INT_MAX;
    for (int i = 0; i < playerNum; i++) {
        if (!ftag[i]) {
            std::vector<Card> handCards = getFinalHands(i);
            int rank = advancedEvaluate(handCards);
            if (rank >= 0 && rank < bestRank) {
                res.clear();
                bestRank = rank;
                res.push_back(i);
            } else if (rank >= 0 && rank == bestRank) {
                res.push_back(i);
            }
        }
    }
    return res;
}