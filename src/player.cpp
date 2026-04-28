#include "player.h"

inline std::string action2str(const ACTION& action) {
    static const std::string actionStr[] = {" Fold ", "Check ", " Call ", " Bet  ", "Raise ", "All-in"};
    return actionStr[action];
}

Player::Player(const std::string &name, int startingChips): name{name}, chips{startingChips} {
    hand = {};
}

void Player::setHand(const std::vector<Card> &cards) {
    hand = {};
    if (cards.size() == 2)
        hand = cards;
}
