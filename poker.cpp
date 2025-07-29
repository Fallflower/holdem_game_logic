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
    {'A', A}
};

const std::string num_str[] = {
    "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"
};

const std::string suit_str[] = {
    "\u2665", "\u2663", "\u2666", "\u2660"
};
Poker::Poker(CARDNUM c, SUIT s) : suit(s), cnum(c) {}

std::string Poker::to_string() const {
    return num_str[cnum] + suit_str[suit];
}

bool operator>(const Poker& p1, const Poker& p2) {
    return p1.cnum > p2.cnum;
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