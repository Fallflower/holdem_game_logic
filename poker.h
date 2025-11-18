#ifndef __POKER_H__
#define __POKER_H__
#include<iostream>
#include<string>
#include<map>
enum SUIT
{
	HEA,	// 红桃
	CLU,	// 梅花
	DIA,	// 方片
	SPA		// 黑桃
};

extern const std::map<char, SUIT> smap;
extern const std::string suit_str[];

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
    ACE
};

extern const std::map<char, CARDNUM> numap;
const std::string num_str(const CARDNUM&);

struct Poker
{
private:
	SUIT suit;
	CARDNUM cnum;
public:
	Poker(CARDNUM c = ACE, SUIT s = HEA);
	std::string to_string() const;

	SUIT getSuit() const;
	CARDNUM getNum() const;


	friend bool operator>(const Poker &p1, const Poker &p2);
	friend bool operator<(const Poker &p1, const Poker &p2);
    std::ostream& output(std::ostream& out) const;
};

std::ostream &operator<<(std::ostream &out, const Poker &p);
#endif