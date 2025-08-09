#include "poker.h"

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

const std::string num_str(const CARDNUM& c) {
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

const std::string suit_str[] = {
    "\u2665", "\u2663", "\u2666", "\u2660"
};
Poker::Poker(CARDNUM c, SUIT s) : suit(s), cnum(c) {}

std::string Poker::to_string() const {
    return num_str(cnum) + suit_str[suit];
}

SUIT Poker::getSuit() const {
    return suit;
}

CARDNUM Poker::getNum() const {
    return cnum;
}

bool operator>(const Poker& p1, const Poker& p2) {
    return p1.cnum > p2.cnum;
}

bool operator<(const Poker &p1, const Poker &p2) {
    return p1.cnum < p2.cnum;
}

std::ostream &Poker::output(std::ostream &out) const
{
    out << to_string();
    return out;
}

std::ostream &operator<<(std::ostream &out, const Poker &p)
{
    p.output(out);
    return out;
}