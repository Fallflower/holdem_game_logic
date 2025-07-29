#include "game.h"

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
    pile = new Poker[52];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
            pile[i*13+j] = Poker(CARDNUM(j), SUIT(i));
    
    chips = new int[playerNum]{0};
    ftag = new bool[playerNum]{0};
    active = (dealer + 3) % playerNum;

    // commit blinds
    int sb = pos.find(" S B ");
    int bb = pos.find(" B B ");
    chips[sb] = 1;
    chips[bb] = 2;
}

void Game::shuffle() {
    std::random_device seed;
    std::ranlux48 engine(seed());
    std::uniform_int_distribution<> distrib(0, 51);
    for (int i = 0; i < 52; i++)
    {
        int r;
        while ((r = distrib(engine)) == i);
        Swap(pile[i], pile[r]);
    }
}

void Game::dealCards () {
    hands = new Poker*[playerNum];
    for (int i = 0; i < playerNum; i++)
    {
        int pi = (dealer + i) % playerNum; // deal begin from dealer
        hands[pi] = new Poker[2];
        if (pile[i] > pile[i + playerNum]) {
            hands[pi][0] = pile[i];
            hands[pi][1] = pile[i + playerNum];
        } else {
            hands[pi][1] = pile[i];
            hands[pi][0] = pile[i + playerNum];
        }
    }
    int j = 2 * playerNum;
    for (int k = 0; k < 3; k++)
        flop[k] = pile[j + k + 1];
    turn = pile[j + 5];
    river = pile[j + 7];
}

std::string Game::genPubCardStr() const {
    std::vector<std::string> temp = {"??", "??", "??", "??", "??"};
    switch (stateCode)
    {
    case 3: temp[4] = river.to_string(); [[fallthrough]];
    case 2: temp[3] = turn.to_string(); [[fallthrough]];
    case 1:
        temp[0] = flop[0].to_string();
        temp[1] = flop[1].to_string();
        temp[2] = flop[2].to_string();
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


Game::Game(int pn, int d): playerNum(pn), dealer(d) {
    init();
    shuffle();
    dealCards();
    stateCode = 3;
}

Game::~Game() {
    delete[] chips;
    delete[] ftag;
    delete[] pile;

    for (int i = 0; i < playerNum; i++)
        delete[] hands[i];
    delete[] hands;
}


void Game::show() const {
    std::cout << "================================================================" << std::endl;
    std::cout << "  Public: " << std::endl;
    std::cout << "\t\t\t" << genPubCardStr() << std::endl;
    std::cout << "  State:  " << stateStr[stateCode] << std::endl;
    std::cout << "----------------------------------------------------------------" << std::endl;
    for (int i = 0; i < playerNum; i++) {
        if (i == active) std::cout << " *";
        else std::cout << "  ";
        std::cout << "Player" << i + 1 << " (" << pos[i] << "):   ";
        for (int j = 0; j < 2; j++)
            std::cout << hands[i][j] << ' ';
        std::cout << "\t" << chips[i] << "\t\tWin: %" << std::endl;
    }
    std::cout << "================================================================" << std::endl;
}

int Game::getPot() const {
    int temp = 0;
    for (int i = 0; i < playerNum; i++)
        temp += chips[i];
    return temp;
}

void Game::bet(const int& chip) {
    chips[active] = chip;
    active = (active + 1) % playerNum;
}
