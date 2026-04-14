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
	
	void setChips(const int&);
	void fold();		  // set folded=true
	void takeAction();

	int getChips() const;
	std::string getName() const;
	std::vector<Card> getHand() const;

};


#endif