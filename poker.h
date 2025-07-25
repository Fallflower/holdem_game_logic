#ifndef __POKER_H__
#define __POKER_H__
#include<iostream>
#include<map>
enum SUIT
{
	HEA, CLU, DIA, SPA
};

extern const std::map<char, SUIT> smap;

enum CARDNUM
{
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,
	NUM_10,
	JACK,
	QUEEN,
	KING,
    A
};

extern const std::map<char, CARDNUM> numap;

struct Poker
{
private:
	SUIT suit;
	CARDNUM cnum;
public:
	Poker( CARDNUM c = A, SUIT s = HEA);
	friend bool operator>(const Poker &p1, const Poker &p2);
    std::ostream& output(std::ostream& out) const;
};

std::ostream &operator<<(std::ostream &out, const Poker &p);
#endif