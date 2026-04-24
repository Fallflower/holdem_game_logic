#ifndef __HAND_TYPE_H__
#define __HAND_TYPE_H__
#include "card.h"
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>

enum HANDRANK {
    HIGH_CARD,      // 高牌
    ONE_PAIR,       // 一对
    TWO_PAIR,       // 两对
    THREE_OF_A_KIND,// 三条
    STRAIGHT,       // 顺子
    FLUSH,          // 同花
    FULL_HOUSE,     // 葫芦
    FOUR_OF_A_KIND, // 四条
    STRAIGHT_FLUSH  // 同花顺
};

struct HandType
{
    HANDRANK rank;
    std::vector<CARDNUM> keys;

    std::string to_string() const;
    static HandType evaluate(const std::vector<Card>& cards);
    static int compareHandType(const HandType& t1, const HandType& t2);
};

struct advancedHandType
{
    // Load pre-split binary file (flush_map + other_map sections)
    bool load(const std::string& bin_path);
    // Convert a raw full hash→rank map into split flush_map/other_map
    void convert(std::unordered_map<uint64_t, int>& strength_map);
    // Lookup: try flush_map first, then ranksHash + other_map
    int operator[](const uint64_t& hash);

    std::unordered_map<uint64_t, int> flush_map, other_map;
    bool loaded = false;
};

int advancedEvaluate(const std::vector<Card>& cards); //通过查表评估牌型，返回一个整数，整数越大牌型越好
std::ostream& operator<<(std::ostream& out, const HandType& t);

#endif