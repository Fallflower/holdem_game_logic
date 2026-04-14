#include "player.h"
Player::Player(const std::string &name, int startingChips):name{name}, chips{startingChips} {
    hand = {};
}

void Player::setHand(const std::vector<Card> &cards) {
    hand = {};
    if (cards.size() == 2)
        hand = cards;
}

void Player::setChips(const int& amount) {
    chips = amount;
}

void takeAction();

int Player::getChips() const {
    return chips;
}
std::string Player::getName() const {
    return name;
}

std::vector<Card> Player::getHand() const {
    return hand;
}
