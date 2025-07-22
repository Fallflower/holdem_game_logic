#include "game.h"

template <class ElemType >
inline void Swap(ElemType &e1, ElemType &e2)
// 操作结果: 交换e1, e2之值
{
	ElemType temp;		// 临时变量
	// 循环赋值实现交换e1, e2
	temp = e1;	e1 = e2;  e2 = temp;
}

void Game::initPosition() {
    pos = Position(playerNum, dealer);
}

void Game::initPile() {
    pile = new Poker[52];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
            pile[i*13+j] = Poker(CARDNUM(j), SUIT(i));
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
        hands[pi][0] = pile[i];
        hands[pi][1] = pile[i+playerNum];
    }
    int j = 2 * playerNum;
    for (int k = 0; k < 3; k++)
        flop[k] = pile[j + k + 1];
    turn = pile[j + 5];
    river = pile[j + 7];
}


Game::Game(int pn, int d): playerNum(pn), dealer(d) {
    initPosition();
    initPile();
    shuffle();
    dealCards();
    stateCode = 0;
}

Game::~Game() {
    delete[] pile;
    for (int i = 0; i < playerNum; i++)
        delete[] hands[i];
    delete[] hands;
}


void Game::show() const {
    std::cout << "================================================================" << std::endl;
    std::cout << "  Public: " << std::endl;
    std::cout << "\t\t\t  ";
    for (int i = 0; i < 3; i++) {
        std::cout << flop[i] << ' ';
    }
    std::cout << turn << ' ' << river << std::endl;
    std::cout << "\n----------------------------------------------------------------" << std::endl;
    for (int i = 0; i < playerNum; i++) {
        std::cout << "  Player" << i + 1 << " (" << pos[i] << "):   ";
        for (int j = 0; j < 2; j++)
            std::cout << hands[i][j] << ' ';
        std::cout << "\t\t\t\tWin: %" << std::endl;
    }
    std::cout << "================================================================" << std::endl;
}