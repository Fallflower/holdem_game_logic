#include "game.h"

template <class ElemType >
void Swap(ElemType &e1, ElemType &e2)
// 操作结果: 交换e1, e2之值
{
	ElemType temp;		// 临时变量
	// 循环赋值实现交换e1, e2
	temp = e1;	e1 = e2;  e2 = temp;
}

void Game::init() {
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


Game::Game() {
    init();


}

Game::~Game() {
    delete[] pile;
}


void Game::show() const {
    for (int i = 0; i < 52; i++)
    {
        std::cout << pile[i] << std::endl;
    }
    
}