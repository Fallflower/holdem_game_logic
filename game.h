#ifndef __GAME_H__
#define __GAME_H__
#include "poker.h"
#include "position.h"
#include<random>
class Game
{
private:
    Poker* pile;
    Poker flop[3];
    Poker turn;
    Poker river;
    Poker **hands;

    int playerNum;
    int stateCode;  // 0, 1, 2, 3
    int dealer;
    Position pos;

    void initPosition();
    void initPile();
    void shuffle();
    void dealCards();

public:
    Game(int pn = 3, int d = 0);
    ~Game();

    void show() const;
};
#endif