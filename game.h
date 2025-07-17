#ifndef __GAME_H__
#define __GAME_H__
#include "poker.h"
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

    void init();
    void shuffle();
    void dealCards(int pn);
public:
    Game(int pn = 3);
    ~Game();

    void show() const;
};
#endif