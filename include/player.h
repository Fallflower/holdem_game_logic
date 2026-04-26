#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "card.h"
#include <vector>

enum ACTION {
	FOLD,
	CHECK,
	CALL,
	BET,
	RAISE,
	ALLIN
};

class Player {
protected:
    std::string name;
    std::vector<Card> hand;
    int chips;

public:
	Player(const std::string &name, int startingChips);

	void setName(const std::string &newName) { name = newName; }
	void setHand(const std::vector<Card> &cards);
	
	void setChips(const int& amount) {chips = amount; }
	void addChips(const int& amount) {chips += amount;}
	virtual ACTION makeAction(const int&, int &) = 0;

	int getChips() const { return chips; }
	std::string getName() const { return name; }
	std::vector<Card> getHand() const { return hand; }
};


#endif