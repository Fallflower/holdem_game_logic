#ifndef __GAME_H__
#define __GAME_H__
#include "handType.h"
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
    int dealer;
    int stateCode;  // 0, 1, 2, 3
    int commit[4] = {2, 0, 0, 0};  // Chips commitment of each round (2 means big blind)
    Position pos;

    int active;     // index: active player
    int lastBet;    // index: the last player who bet
    int *chips;     // chips commitment of each player
    bool *ftag;     // fold tags

    void init();
    void shuffle();
    void dealCards();
    void checkState();

    void step();    // move "active"

    std::string genPubCardStr() const;
    std::vector<Poker> getHands(const int&) const;
public:
    Game(int pn = 3, int d = 0, int s = 0);
    ~Game();

    void show() const;
    int getPot() const;
    int getChipsToCall() const;
    int getState() const;
    void fold();
    void call();     // return the num of chips should be added
    void bet(const int&);

    std::vector<int> checkWinner() const;

};
#endif