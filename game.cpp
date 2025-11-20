#include "game.h"

const std::string stateStr[] = {"preflop", "flop", "turn", "river", "end"};

template <class ElemType >
inline void Swap(ElemType &e1, ElemType &e2)
// 操作结果: 交换e1, e2之值
{
	ElemType temp;		// 临时变量
	// 循环赋值实现交换e1, e2
	temp = e1;	e1 = e2;  e2 = temp;
}

void Game::init() {
    pos = Position(playerNum, dealer);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
            pile.push_back(Poker(CARDNUM(j), SUIT(i)));
    chips = new int[playerNum]{0};
    ftag = new bool[playerNum]{0};
    active = (dealer + 3) % playerNum;
    // commit blinds
    int sb = pos.find(" S B ");
    int bb = pos.find(" B B ");
    chips[sb] = 1;
    chips[bb] = 2;
    lastBet = bb;
}

void Game::shuffle() {
    std::random_device seed;
    std::ranlux48 engine(seed());
    // std::uniform_int_distribution<> distrib(0, 51);
    // for (int i = 0; i < 52; i++)
    // {
    //     int r;
    //     while ((r = distrib(engine)) == i);
    //     Swap(pile[i], pile[r]);
    // }
    std::shuffle(pile.begin(), pile.end(), engine);
}

void Game::dealCards () {
    for (int i = 0; i < playerNum; i++)
        if (pile[i] > pile[i + playerNum])
            hands.push_back({pile[i], pile[i + playerNum]});
        else
            hands.push_back({pile[i + playerNum], pile[i]});
    int j = 2 * playerNum;
    pubCards.assign(pile.begin() + j, pile.begin() + j + 5);
}

void Game::checkState() {
    int i, pi;
    for (i = playerNum - 1, pi = (lastBet + i) % playerNum; i >= 0; i--)    // 反向检查
        if (!ftag[i] && chips[pi] < commit[stateCode])      // pi 从lastBet的前一位开始
            break;
    if (i == -1) {
        int bb = pos.find(" B B ");
        if (stateCode == 0 && bb == lastBet && chips[bb] == 2) // 针对f翻前桌call大盲的情况特殊处理：大盲位还有说话的机会
            return;
        else {
            stateCode++;
            active = (dealer + 1) % playerNum;
            step();
        }
    }
}

void Game::step() {
    while (ftag[active]) {
        active = (active + 1) % playerNum;
    }
}

std::string Game::genPubCardStr() const {
    std::vector<std::string> temp = {"??", "??", "??", "??", "??"};
    switch (stateCode)
    {
    case 3: temp[4] = pubCards[4].to_string(); [[fallthrough]];
    case 2: temp[3] = pubCards[3].to_string(); [[fallthrough]];
    case 1:
        temp[0] = pubCards[0].to_string();
        temp[1] = pubCards[1].to_string();
        temp[2] = pubCards[2].to_string();
        break;
    default:
        break;
    }
    std::ostringstream oss;
    for (size_t i = 0; i < temp.size(); i++)
    {
        if (i > 0) oss << "  ";
        oss << temp[i];
    }
    return oss.str();
}

std::vector<Poker> Game::getHands(const int& k) const {
    std::vector<Poker> temp;
    if (stateCode)
        temp.assign(pubCards.begin(), pubCards.end() - 3 + stateCode);
    temp.insert(temp.end(), hands[k].begin(), hands[k].end());
    return temp;
}

std::vector<double> Game::calcPreflopWinRate(const int& simulations) const {
    std::vector<double> win(playerNum, 0.0);
    std::vector<Poker> deck(pile.begin() + 2 * playerNum, pile.end()); // 取除了手牌以外的牌
    std::random_device seed;
    std::mt19937 engin(seed());
    for (int i = 0; i < simulations; i++)
    {
        auto tmp = deck;
        std::shuffle(tmp.begin(), tmp.end(), engin);
        if (tmp.size() < 5) std::cerr << "fuck " << std::endl;
        std::vector<Poker> board(tmp.begin(), tmp.begin() + 5);

        auto winners = checkWinner(board);

        int n = winners.size();
        for (auto j : winners)
            win[j] += 1.0 / n;
    }
    for (int i = 0; i < playerNum; i++)
        win[i] = 100.0 * win[i] / simulations;
    return win;
}

std::vector<int> Game::checkWinner(std::vector<Poker> public_cards) const {
    std::vector<int> res;
    HandType bestType{HIGH_CARD, {NUM_2}};
    for (int i = 0; i < playerNum; i++) {
        if (!ftag[i]) {
            std::vector<Poker> handCards = hands[i];
            handCards.insert(handCards.end(), public_cards.begin(), public_cards.end());
            HandType t = evaluate(handCards);
            switch (compareHandType(t, bestType))
            {
            case 1:
                res.clear(); bestType = t; [[fallthrough]];
            case 0:
                res.push_back(i); break;
            default:
                break;
            }
        }
    }
    return res;
}

Game::Game(int pn, int d, int s): playerNum(pn), dealer(d), stateCode(s) {
    init();
    shuffle();
    dealCards();
    // stateCode = 3;  // set to 3 (river state) for test
}

Game::~Game() {
    delete[] chips;
    delete[] ftag;
}


void Game::show() const {
    std::cout << "================================================================" << std::endl;
    std::cout << "  Public: " << std::endl;
    std::cout << "\t\t\t" << genPubCardStr() << std::endl;
    std::cout << "   State:  " << stateStr[stateCode] << std::endl;
    std::cout << "     Pot:  " << getPot() << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;
    std::vector<double> win_rate;
    try {
        win_rate = calcPreflopWinRate(20000);
    }
    catch (const std::exception& e) {
        std::cout << "Fatal error: " << e.what() << std::endl;
    }
    for (int i = 0; i < playerNum; i++) {
        if (i == active) std::cout << " *";
        else std::cout << "  ";
        std::cout << "Player" << i + 1 << " (" << pos[i] << "):   ";
        for (int j = 0; j < 2; j++)
            std::cout << hands[i][j] << ' ';
        std::cout << "\t" << chips[i];
        if (ftag[i])
            std::cout << "\t(fold)\t\t" << evaluate(getHands(i));
        else
            std::cout << "\t" << "胜率: " << std::fixed << std::setprecision(2) << win_rate[i] << "%\t" << evaluate(getHands(i));
            // std::cout << "\t\tWin: %";
        std::cout << std::endl;
    }
    std::cout << "================================================================" << std::endl;
}

int Game::getPot() const {
    int temp = 0;
    for (int i = 0; i < playerNum; i++)
        temp += chips[i];
    return temp;
}

int Game::getChipsToCall() const {
    return commit[stateCode] - chips[active];
}

int Game::getState() const {
    return stateCode;
}

void Game::fold() {
    ftag[active] = 1;
    step();
    checkState();
}

void Game::call() {
    int rest = commit[stateCode] - chips[active];
    if (active == pos.find(" B B ") && chips[active] == 2 && rest == 0) { // 针对翻前桌call大盲的情况特殊处理：大盲选择check
        stateCode++;
        active = (dealer + 1) % playerNum;
        step();
        return;
    }
    chips[active] = commit[stateCode];
    active = (active + 1) % playerNum;
    step();
    checkState();
}

void Game::bet(const int& chip) {
    if (chips[active] + chip <= commit[stateCode]) {
        std::cerr << "Invalid bet operation" << std::endl;
    } else {
        chips[active] += chip;
        commit[stateCode] = chips[active];
    }
    lastBet = active;
    active = (active + 1) % playerNum;
    step();
}

std::vector<int> Game::checkWinner() const {
    std::vector<int> res;
    HandType bestType{HIGH_CARD, {NUM_2}};
    for (int i = 0; i < playerNum; i++) {
        if (!ftag[i]) {
            std::vector<Poker> handCards = getHands(i);
            HandType t = evaluate(handCards);
            switch (compareHandType(t, bestType))
            {
            case 1:
                res.clear();    [[fallthrough]];
            case 0:
                res.push_back(i); bestType = t;   break;
            default:
                break;
            }
        }
    }
    return res;
}