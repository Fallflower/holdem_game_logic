#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "card.h"
#include <vector>

class Player {
private:
    std::string name;
    std::vector<Card> hand;
    int chips;

public:
	Player(const std::string &name, int startingChips);

	void setHand(const std::vector<Card> &cards); // Add a card to the hand
	void clearHand();					// Reset hand (for new round)
	
	void setChips(const int& amount) {chips = amount; }
	void addChips(const int& amount) {chips += amount;}
	void takeAction();

	int getChips() const { return chips; }
	std::string getName() const { return name; }
	std::vector<Card> getHand() const { return hand; }

	void showActionMenu(const int& chipsToCall) const;
};


#endif