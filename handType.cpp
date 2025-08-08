#include "handType.h"

HandType evaluate(const std::vector<Poker>& cards) {
    std::map<SUIT, int> suit_statistic = {
        {HEA, 0},
        {CLU, 0},
        {DIA, 0},
        {SPA, 0}
    };
    std::map<CARDNUM, int> num_statistic;

    for (CARDNUM i = NUM_2; i <= ACE; i = CARDNUM(i + 1))
        num_statistic[i] = 0;
    
    for (Poker c : cards) {
        suit_statistic[c.getSuit()]++;
        num_statistic[c.getNum()]++;
    }
    bool flush = 0, straight = 0, trible = 0, quadra = 0;
    int pair = 0;
    CARDNUM triNum = NUM_2, quaNum= NUM_2, pairNum[2], straiNum= NUM_2; // 最多两对（不含三条及以上）
    std::vector<CARDNUM> highNum;
    
    // check flush
    for (auto it : suit_statistic)
        if (it.second >= 5) {
            flush = 1;
        }

    // check others
    if (num_statistic[NUM_2]
        && num_statistic[NUM_3]
        && num_statistic[NUM_4]
        && num_statistic[NUM_5]
        && num_statistic[ACE]
    ) {
        straight = 1;
        straiNum = NUM_5;
    }

    CARDNUM strai_low = NUM_2;
    int k = 0;
    for (CARDNUM i = NUM_2; i <= ACE; i = CARDNUM(i + 1)) {
        switch (num_statistic[i])
        {
        case 4:
            quadra = 1;
            quaNum = i;
            break;
        case 3:
            trible = 1;
            triNum = i;
            break;
        case 2:
            pairNum[pair++] = i;
            break;
        case 0:
            k = 0;
            strai_low = CARDNUM(i + 1);
            break;
        default:
            break;
        }
        if (num_statistic[i] >= 1) {
            if (i == strai_low + 1) { //    判断顺子
                strai_low = i;
                k++;
            }
            if (num_statistic[i] == 1) {//  记录kicker
                if (highNum.size() == 5)
                    highNum.pop_back();
                highNum.insert(highNum.begin(), i);
            }
        }
        if (k >= 4) {
            straight = 1;
            straiNum = strai_low;
        }
    }

    // return result
    if (flush && straight) return {STRAIGHT_FLUSH, {straiNum}};
    if (quadra) return {FOUR_OF_A_KIND, {quaNum, highNum[0]}};
    if (trible && pair) return {FULL_HOUSE, {triNum, pairNum[pair - 1]}};
    if (flush) return {FLUSH, {}};
    if (straight) return {STRAIGHT, {straiNum}};
    if (trible) return {THREE_OF_A_KIND, {triNum, highNum[0], highNum[1]}};
    if (pair == 2) return {TWO_PAIR, {pairNum[1], pairNum[0], highNum[0]}};
    if (pair == 1) return {ONE_PAIR, {pairNum[0], highNum[0], highNum[1], highNum[2]}};
    return {HIGH_CARD, highNum};
}

static int compareHandType(const HandType& t1, const HandType& t2) {
    if (t1.rank != t2.rank) {
        return t1.rank > t2.rank ? 1 : -1;
    }

    for (size_t i = 0; i < t1.keys.size(); i++) {
        if (t1.keys[i] != t2.keys[i]) {
            return t1.keys[i] > t2.keys[i] ? 1 : -1;
        }
    }
    return 0;
}

std::ostream& operator<<(std::ostream& out, const HandType& t) {
    out << int(t.rank) << std::endl;
    for (auto i : t.keys) {
        out << int(i) << "\t";
    }
    out << std::endl;
    return out;
}