#include "player.h"
#include <iomanip>

Player::Player(const std::string &name, int startingChips):name{name}, chips{startingChips} {
    hand = {};
}

void Player::setHand(const std::vector<Card> &cards) {
    hand = {};
    if (cards.size() == 2)
        hand = cards;
}

void Player::showActionMenu(const int& chipsToCall) const
{
	std::cout << "==========================" << std::endl;
    if(chipsToCall == 0) {
	    std::cout << "=== Check          [1] ===" << std::endl;
    	std::cout << "=== Bet            [2] ===" << std::endl;
    } else if ( chipsToCall < chips) {
        std::cout << "=== Fold           [1] ===" << std::endl;
        std::cout << "=== Call  " << std::right << std::setw(5) << chipsToCall << " BB [2] ===" << std::endl;
    	std::cout << "=== Raise          [3] ===" << std::endl;
    } else {
        std::cout << "=== Fold           [1] ===" << std::endl;
        std::cout << "=== Allin to Call  [2] ===" << std::endl;
    }
	std::cout << "=== QUIT         [Esc] ===" << std::endl;
	std::cout << "==========================" << std::endl;
}
