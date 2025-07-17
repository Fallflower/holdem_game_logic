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

    void init();
    void shuffle();
public:
    Game();
    ~Game();
};
#endif