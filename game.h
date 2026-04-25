#ifndef __GAME_H__
#define __GAME_H__
#include "handType.h"
#include "deck.h"
#include "position.h"
#include "player.h"
#include<random>
#include<sstream>
#include<iomanip>

extern const std::string stateStr[];

class Game
{
private:
    std::vector<std::vector<Card>> hands;
    Deck deck_;

    int playerNum;
    std::vector<Player> players;
    int hpi;        // humanPlayerIndex
    int dealer;
    int stateCode;  // 0, 1, 2, 3
    int commit[4] = {2, 0, 0, 0};  // Chips commitment of each round (2 means big blind)
    Position pos;

    int active;     // index: active player
    int lastBet;    // index: the last player who bet
    int **chips;    // [playerNum, 4] chips commitment of each player at each round
    bool *ftag;     // fold tags
    bool *ctag;     // check tags

    bool _end;      // all fold

    void init_game();
    void init_players(const Player&, const int&);
    void checkState();

    int getCommited(const int&) const;

    void step();    // move "active"

    std::vector<Card> getHands(const int&) const;   // public + hand

    std::vector<double> calcWinRate(const int& simulations = 12288) const;
    std::vector<int> checkWinner(std::vector<Card> public_cards) const;
public:
    Game(int pn = 3, int d = 0);
    Game(const Position& posInfo, const int& initialChips, const Player& humanPlayer, const int &humanPlayerPosIndex);
    ~Game();

    void show() const;
    void showPlayerView() const;
    int getPot() const;
    int getChipsToCall() const;    // return the num of chips should be added
    int getState() const;
    void fold();
    void call();     
    void bet(const int&);

    bool isEnd() const;
    Player getPlayer(const int& pi) const;
    std::vector<int> checkWinner() const;

    Position getPosiInfo() const;

};
#endif