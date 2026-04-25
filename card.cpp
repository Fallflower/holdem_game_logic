#include "card.h"

const std::map<char, SUIT> smap = {
    {'H', HEA},
    {'C', CLU},
    {'D', DIA},
    {'S', SPA}
};

const std::map<char, CARDNUM> numap = {
    {'2', NUM_2},
    {'3', NUM_3},
    {'4', NUM_4},
    {'5', NUM_5},
    {'6', NUM_6},
    {'7', NUM_7},
    {'8', NUM_8},
    {'9', NUM_9},
    {'T', NUM_10},
    {'J', JACK},
    {'Q', QUEEN},
    {'K', KING},
    {'A', ACE}
};

const std::string num2str(const CARDNUM& c) {
    switch (c)
    {
    case NUM_2: return "2"; 
    case NUM_3: return "3";
    case NUM_4: return "4";
    case NUM_5: return "5";
    case NUM_6: return "6";
    case NUM_7: return "7";
    case NUM_8: return "8";
    case NUM_9: return "9";
    case NUM_10: return "T";
    case JACK: return "J";
    case QUEEN: return "Q";
    case KING: return "K";
    case ACE: return "A";
    }
    return "";
}

const std::string suit2str(const SUIT& s) {
    switch (s) {
    case HEA: return "\u2665";
    case CLU: return "\u2663";
    case DIA: return "\u2666";
    case SPA: return "\u2660";
    }
    return "";
}

Card::Card(CARDNUM c, SUIT s) : suit(s), cnum(c) {}

std::string Card::to_string() const {
    // ANSI color: red for hearts/diamonds, black for clubs/spades
    std::string color = (suit == HEA || suit == DIA) ? "\033[31m" : "\033[30m";
    std::string bg = "\033[47m";
    std::string reset = "\033[0m";

    // Rank: "10" for NUM_10, space-padded single char for others (2-wide)
    std::string rank = (cnum == NUM_10) ? "10" : " " + num2str(cnum);

    return color + bg + "[" + rank + suit2str(suit) + "]" + reset;
}

SUIT Card::getSuit() const {
    return suit;
}

CARDNUM Card::getNum() const {
    return cnum;
}

int Card::toInt() const {
    return int(cnum) * 4 + int(suit);
}

uint64_t Card::toLong() const {
    return (uint64_t)(1) << this->toInt();
}

uint64_t Card::cardsToLong(const std::vector<Card>& cards) {
    uint64_t res = 0;
    for (const Card& c : cards) {
        res |= c.toLong();      // 按位或运算更robust，避免重复卡牌导致的错误
    }
    return res;
}

bool operator>(const Card& p1, const Card& p2) {
    return p1.getNum() > p2.getNum();
}

bool operator<(const Card &p1, const Card &p2) {
    return p1.getNum() < p2.getNum();
}

bool operator==(const Card &p1, const Card &p2) {
    return p1.getNum() == p2.getNum() && p1.getSuit() == p2.getSuit();
}
std::ostream &Card::output(std::ostream &out) const
{
    out << to_string();
    return out;
}

std::ostream &operator<<(std::ostream &out, const Card &p)
{
    p.output(out);
    return out;
}