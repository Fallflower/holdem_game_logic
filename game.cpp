#include "game.h"

const std::string stateStr[] = {"preflop", "flop", "turn", "river", "end"};
class Error : public std::exception
{
private:
	unsigned int m_code;
	std::string m_what;
	std::string full_mes;
public:
	Error(unsigned int code,  const std::string &what_arg): m_code(code), m_what(what_arg){
		full_mes =  "Error code: " + std::to_string(m_code) + "\nError message: " + m_what;
	}
	virtual const char * what(void) const noexcept override
	{
		// char* mesg = new char[text.length()+1];
		// strcpy(mesg, text.c_str());
		return full_mes.c_str();
	}
};

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
    // commit blinds
    int sb = pos.find(" S B ");
    int bb = pos.find(" B B ");
    chips[sb][0] = 1;
    chips[bb][0] = 2;
    lastBet = bb;
}

void Game::shuffle() {
    std::random_device seed;
    std::ranlux48 engine(seed());
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

int Game::getCommited(const int& pi) const
{
    int sum = 0;
    for (int i = 0; i <= stateCode; i++)
        sum += chips[pi][i];
    return sum;
}

void Game::step() {
    // std::cout << "active move" << std::endl;
    active = (active + 1) % playerNum;
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

std::vector<double> Game::calcWinRate(const int& simulations) const {
    std::vector<double> win(playerNum, 0.0);
    if (stateCode < 3) {// 使用蒙特卡洛算法计算翻前、翻牌、转牌阶段胜率
        int known_pub_cards_num = 0;
        if (stateCode) known_pub_cards_num = stateCode + 2;
        std::vector<Poker> deck(pile.begin() + 2 * playerNum + known_pub_cards_num, pile.end()); // 取除了手牌和已发出的公牌以外的牌
        // deck.insert(deck.begin(), pubCards.begin(), pubCards.begin() + n);
        // std::cout << deck.size() << std::endl;
        std::random_device seed;
        std::mt19937 engin(seed());
        for (int i = 0; i < simulations; i++)
        {
            auto tmp = deck;
            std::shuffle(tmp.begin(), tmp.end(), engin);
            std::vector<Poker> board(tmp.begin(), tmp.begin() + 5 - known_pub_cards_num);
            board.insert(board.end(), pubCards.begin(), pubCards.begin() + known_pub_cards_num);
            // std::cout << board.size() << std::endl;
            auto winners = checkWinner(board);

            int n = winners.size();
            for (auto j : winners)
                win[j] += 1.0 / n;
        }
    }
    if (stateCode == 3) {   // 河牌阶段，朴实无华的求胜率
            auto winners = checkWinner();
            int n = winners.size();
            if (n == 1) {win[winners[0]] = 1.0 * simulations;} // 一名胜利者胜率显示100%；多名胜者胜率显示0%
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

Game::Game(int pn, int d, int s): playerNum(pn), dealer(d), stateCode(s), _end(0) {
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
    auto win_rate = calcWinRate(20000);
    for (int i = 0; i < playerNum; i++) {
        if (i == active) std::cout << " *";
        else std::cout << "  ";
        std::cout << "Player" << i + 1 << " (" << pos[i] << "):   ";
        for (int j = 0; j < 2; j++)
            std::cout << hands[i][j] << ' ';
        std::cout << "\t" << chips[i][stateCode];
        if (ftag[i])
            std::cout << "\t(fold)\t\t" << evaluate(getHands(i));
        else
            std::cout << "\t" << "胜率: " << std::fixed << std::setprecision(2) << win_rate[i] << "%\t" << evaluate(getHands(i));
        std::cout << std::endl;
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

int Game::getChipsToCall() const {
    return commit[stateCode] - chips[active][stateCode];
}

int Game::getState() const {
    return stateCode;
}

void Game::fold() {
    ftag[active] = 1;
    int num = 0;
    for (int i = 0; i < playerNum; i++)
        if (!ftag[i]) num++;
    if (num == 1) {_end = 1; return;}
    
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

bool Game::isEnd() const {
    if (stateCode > 3) return 1;
    return _end;
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