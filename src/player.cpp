#include "player.h"

Player::Player(const std::string &name, int startingChips): name{name}, chips{startingChips} {
    hand = {};
}

void Player::setHand(const std::vector<Card> &cards) {
    hand = {};
    if (cards.size() == 2)
        hand = cards;
}
