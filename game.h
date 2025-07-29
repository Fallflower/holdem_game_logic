#ifndef __GAME_H__
#define __GAME_H__
#include "poker.h"
#include "position.h"
#include<random>
#include<sstream>

std::map<int, std::string> stateStr = {
    {0, "preflop"},
    {1, "flop"},
    {2, "turn"},
    {3, "river"},
    {4, "end"}
};

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

    std::string genPubCardStr() const;

public:
    Game(int pn = 3, int d = 0);
    ~Game();

    void show() const;
    int getPot() const;
    void bet(const int&);
};
#endif