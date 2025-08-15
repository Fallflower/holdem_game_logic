#include "handType.h"

std::string HandType::to_string() const {
    std::string temp;
    switch (rank)
    {
    case HIGH_CARD:
        temp = std::string("高牌")+num_str(keys[0]);
        break;
    case ONE_PAIR:
        temp = std::string("对")+num_str(keys[0])+" "+num_str(keys[1])+"踢";
        break;
    case TWO_PAIR:
        temp = std::string("对")+num_str(keys[0])+"对"+num_str(keys[1])+" "+num_str(keys[2])+"踢";
        break;
    case THREE_OF_A_KIND:
        temp = std::string("三条")+num_str(keys[0])+" "+num_str(keys[1])+"踢";
        break;
    case STRAIGHT:
        temp = std::string("顺子")+num_str(keys[0]);
        break;
    case FLUSH:
        temp = std::string("同花")+num_str(keys[0]);
        break;
    case FULL_HOUSE:
        temp = num_str(keys[0])+" "+num_str(keys[1])+"的葫芦";
        break;
    case FOUR_OF_A_KIND:
        temp = std::string("四条")+num_str(keys[0])+" "+num_str(keys[1])+"踢";
        break;
    case STRAIGHT_FLUSH:
        if (keys[0] == ACE)
            temp = "皇家同花顺";
        else
            temp = std::string("同花顺")+num_str(keys[0]);
    default:
        break;
    }
    return temp;
}

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
    bool flush = 0, straight = 0, trible = 0, quadra = 0, tag = 0;
    int pair = 0;
    CARDNUM triNum = NUM_2, quaNum= NUM_2, straiNum= NUM_2, flusNum = NUM_2;
    CARDNUM pairNum[3];// 最多三对（不含三条及以上）
    std::vector<CARDNUM> highNum;
    
    // check flush
    for (auto it : suit_statistic)
        if (it.second >= 5) {
            flush = 1;
            std::set<CARDNUM> flush_cards_set;
            for (Poker c : cards)
                if (c.getSuit() == it.first)
                    flush_cards_set.insert(c.getNum());
            std::vector<CARDNUM> flush_cards(flush_cards_set.begin(), flush_cards_set.end());
            std::sort(flush_cards.begin(), flush_cards.end());
            flusNum = flush_cards.back();
            for (size_t i = 0; i <= flush_cards.size() - 5; i++) {
                if (flush_cards[i] == flush_cards[i + 4] + 4) {
                    tag = 1;
                    straiNum = flush_cards[i+4];
                }
            } 
        }

    // check others
    if (num_statistic[NUM_2]
        && num_statistic[NUM_3]
        && num_statistic[NUM_4]
        && num_statistic[NUM_5]
        && num_statistic[ACE]
        && !tag
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
        if (k >= 4 && !tag) {
            straight = 1;
            straiNum = strai_low;
        }
    }

    // return result
    if (flush && straight && tag) return {STRAIGHT_FLUSH, {straiNum}};
    if (quadra) return {FOUR_OF_A_KIND, {quaNum, highNum[0]}};
    if (trible && pair) return {FULL_HOUSE, {triNum, pairNum[pair - 1]}};
    if (flush) return {FLUSH, {flusNum}};
    if (straight) return {STRAIGHT, {straiNum}};
    if (trible) return {THREE_OF_A_KIND, {triNum, highNum[0], highNum[1]}};
    if (pair >= 2) return {TWO_PAIR, {pairNum[pair-1], pairNum[pair-2], highNum[0]}};
    if (pair == 1) {
        std::vector<CARDNUM> kt = {pairNum[0]};
        kt.insert(kt.end(), highNum.begin(), highNum.end());
        return {ONE_PAIR, kt};
    }
    return {HIGH_CARD, highNum};
}

int compareHandType(const HandType& t1, const HandType& t2) {
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
    out << t.to_string();
    return out;
}