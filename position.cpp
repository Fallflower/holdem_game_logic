#include "position.h"

void Position::init(int pn) {
    poss = {" S B ", " B B "};
    switch (pn)
    {
    default: [[fallthrough]];
    case 7:
        poss.insert(poss.begin() + 2, " M P "); [[fallthrough]];
    case 6:
        poss.insert(poss.begin() + 3, " H J "); [[fallthrough]];
    case 5:
        poss.insert(poss.begin() + 4, " C O "); [[fallthrough]];
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


Position::Position(int pn, int di) {
    init(pn);
    adjust(pn, di); // 调整顺序使其与game中的玩家顺序对应
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