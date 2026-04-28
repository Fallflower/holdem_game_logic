#include "position.h"
#include "assistant.h"

/*
 * 根据playerNum初始化位置字符串
 * pn=2 -> {SB, BB}
 * pn=3 -> {SB, BB, D}
 * pn=4 -> {SB, BB, UTG, D}
 * pn=5 -> {SB, BB, UTG, CO, D}
 * pn=6 -> {SB, BB, UTG, HJ, CO, D}
 * pn=7 -> {SB, BB, UTG, MP, HJ, CO, D}
 * pn>7 -> {SB, BB, UTG, UTG+1, ... , MP, HJ, CO, D}
 */
void Position::init(int pn) {
    if (pn < 2)
        throw Error(100, "System Error: Player number must be at least 2.");
    poss = {" S B ", " B B "};
    switch (pn)
    {
    case 2: return;
    default: [[fallthrough]];
    case 7:
        poss.push_back(" M P "); [[fallthrough]];
    case 6:
        poss.push_back(" H J "); [[fallthrough]];
    case 5:
        poss.push_back(" C O "); [[fallthrough]];
    case 4:
        poss.insert(poss.begin() + 2, " UTG "); [[fallthrough]];
    case 3:
        poss.push_back("  D  "); break;// [[fallthrough]];
    }
    if (pn > 7)
        for (int i = 1; i <= pn - 7; i++)
            poss.insert(poss.begin() + 2 + i, "UTG+" + std::to_string(i));
}

void Position::adjust(int pn, int di) {
    std::vector<std::string> temp;
    for (int i = 0; i < pn; i++)
        temp.push_back(poss[(pn - 1 - di + i) % pn]);
    poss = temp; 
}

Position::Position(int pn, int di) : dealer(di) {
    init(pn);
    adjust(pn, di); // 调整顺序使其与game中的玩家顺序对应
}

int Position::find(const std::string& s) const {
    int i;
    for (i = 0; i < int(poss.size()); i++)
        if (s == poss[i]) break;
    return i;   // i == playerNum means failed
}

int Position::getDealer() const {
    return dealer;
}

int Position::getPlayerNum() const {
    return poss.size();
}

std::string Position::operator[](int pi) const {
    return poss[pi];
}




// for test
std::ostream &Position::output(std::ostream &out) const {
    for (auto str:poss)
        out << str << "\t";
    out << std::endl;
    return out;
}

std::ostream &operator<<(std::ostream &out, const Position& p) {
    p.output(out);
    return out;
}