#ifndef __GAME_H__
#define __GAME_H__
#include "handType.h"
#include "position.h"
#include<random>
#include<sstream>
#include<iomanip>

extern const std::string stateStr[];

class Game
{
private:
    std::vector<Poker> pile;
    std::vector<Poker> pubCards;
    std::vector<std::vector<Poker>> hands;

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

    std::vector<double> calcPreflopWinRate(const int& simulations = 20000) const;
    std::vector<int> checkWinner(std::vector<Poker> public_cards) const;
public:
    Game(int pn = 3, int d = 0, int s = 0);
    ~Game();

    void show() const;
    int getPot() const;
    int getChipsToCall() const;    // return the num of chips should be added
    int getState() const;
    void fold();
    void call();     
    void bet(const int&);

    std::vector<int> checkWinner() const;

};
#endif