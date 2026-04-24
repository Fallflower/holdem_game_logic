#ifndef __CARD_H__
#define __CARD_H__
#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <map>

enum SUIT	// 调整为c,d,h,s的顺序，与texas solver的顺序一致
{
	CLU,	// 梅花
	DIA,	// 方片
	HEA,	// 红桃
	SPA		// 黑桃
};

extern const std::map<char, SUIT> smap;
const std::string suit2str(const SUIT&);

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
const std::string num2str(const CARDNUM&);

struct Card
{
private:
	SUIT suit;
	CARDNUM cnum;
public:
	Card(CARDNUM c = ACE, SUIT s = HEA);
	std::string to_string() const;

	SUIT getSuit() const;
	CARDNUM getNum() const;
	int toInt() const;
	uint64_t toLong() const;
	static uint64_t cardsToLong(const std::vector<Card>& cards);
    std::ostream& output(std::ostream& out) const;
};

bool operator>(const Card &p1, const Card &p2);
bool operator<(const Card &p1, const Card &p2);
bool operator==(const Card &p1, const Card &p2);
std::ostream &operator<<(std::ostream &out, const Card &p);
#endif