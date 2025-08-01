#ifndef __GAME_H__
#define __GAME_H__
#include "poker.h"
#include "position.h"
#include<random>
#include<sstream>

extern const std::string stateStr[];

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
    int commit[4] = {2, 0, 0, 0};  // Chips commitment of each round and 2 means big blind
    int dealer;
    Position pos;

    int active;     // active player
    int *chips;     // chips commitment of each player
    bool *ftag;     // fold tags

    void init();
    void shuffle();
    void dealCards();
    void checkState();

    void step();    // move "active"

    std::string genPubCardStr() const;

public:
    Game(int pn = 3, int d = 0);
    ~Game();

    void show() const;
    int getPot() const;
    int getState() const;
    void fold();
    int call();     // return the num of chips should be added
    void bet(const int&);
};
#endif