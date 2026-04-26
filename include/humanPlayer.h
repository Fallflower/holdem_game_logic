#ifndef __HUMAN_PLAYER_H__
#define __HUMAN_PLAYER_H__

#include "player.h"

class HumanPlayer : public Player {
private:
    int showActionMenu(const int& chipsToCall) const;
public:
    HumanPlayer(const std::string &name, int startingChips) : Player(name, startingChips) {}

    ACTION makeAction(const int& chipsToCall, int &betAmount) override;
};


#endif