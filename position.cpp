#include "position.h"

Position::Position(int pn) {
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