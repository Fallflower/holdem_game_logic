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

Poker::Poker(CARDNUM c, SUIT s) : suit(s), cnum(c) {}

bool operator>(const Poker& p1, const Poker& p2) {
    return p1.cnum > p2.cnum;
}

std::ostream &Poker::output(std::ostream &out) const
{
    // 输出点数
    static const char* num_str[] = {
        "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K", "A"
    };
    out << num_str[cnum];
    
    // 输出花色 UTF-8 encoding
    static const char* suit_str[] = {"\u2665", "\u2663", "\u2666", "\u2660"};
    out << suit_str[suit];
    
    return out;
}

std::ostream &operator<<(std::ostream &out, const Poker &p)
{
    p.output(out);
    return out;
}