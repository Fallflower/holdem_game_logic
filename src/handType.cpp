#include "handType.h"
#include <fstream>
#include <climits>

#define SUIT_0_MASK   0x1111111111111ULL
#define SUIT_1_MASK   0x2222222222222ULL
#define SUIT_2_MASK   0x4444444444444ULL
#define SUIT_3_MASK   0x8888888888888ULL
#define BIT_SUM_0     0x5555555555555ULL
#define BIT_SUM_1     0x3333333333333ULL

// Compress rank bits: for each of the 13 ranks, count how many suits are present
static uint64_t ranksHash(uint64_t cards_hash) {
    cards_hash = (cards_hash & BIT_SUM_0) + ((cards_hash >> 1) & BIT_SUM_0);
    cards_hash = (cards_hash & BIT_SUM_1) + ((cards_hash >> 2) & BIT_SUM_1);
    return cards_hash;
}

// Check whether all cards in the bitmask share one suit
static bool isFlush(uint64_t hash) {
    int cnt = (hash & SUIT_0_MASK) != 0;
    cnt += (hash & SUIT_1_MASK) != 0;
    if (cnt > 1) return false;
    cnt += (hash & SUIT_2_MASK) != 0;
    if (cnt > 1) return false;
    cnt += (hash & SUIT_3_MASK) != 0;
    return cnt <= 1;
}

// Global singleton ranker instance
advancedHandType g_advancedRanker;

std::string HandType::to_string() const {
    std::string temp;
    switch (rank)
    {
    case HIGH_CARD:
        temp = std::string("高牌")+num2str(keys[0]);
        break;
    case ONE_PAIR:
        if (keys.size() > 1)
            temp = std::string("对")+num2str(keys[0])+" "+num2str(keys[1])+"踢";
        else
            temp = std::string("对")+num2str(keys[0]);
        break;
    case TWO_PAIR:
        temp = std::string("对")+num2str(keys[0])+"对"+num2str(keys[1])+" "+num2str(keys[2])+"踢";
        break;
    case THREE_OF_A_KIND:
        temp = std::string("三条")+num2str(keys[0])+" "+num2str(keys[1])+"踢";
        break;
    case STRAIGHT:
        temp = std::string("顺子")+num2str(keys[0]);
        break;
    case FLUSH:
        temp = std::string("同花")+num2str(keys[0]);
        break;
    case FULL_HOUSE:
        temp = num2str(keys[0])+" "+num2str(keys[1])+"的葫芦";
        break;
    case FOUR_OF_A_KIND:
        temp = std::string("四条")+num2str(keys[0])+" "+num2str(keys[1])+"踢";
        break;
    case STRAIGHT_FLUSH:
        if (keys[0] == ACE)
            temp = "皇家同花顺";
        else
            temp = std::string("同花顺")+num2str(keys[0]);
    default:
        break;
    }
    return temp;
}

HandType HandType::evaluate(const std::vector<Card>& cards) { //支持2-7张牌的评估
    std::map<SUIT, int> suit_statistic = {
        {HEA, 0},
        {CLU, 0},
        {DIA, 0},
        {SPA, 0}
    };
    std::map<CARDNUM, int> num_statistic;

    for (CARDNUM i = NUM_2; i <= ACE; i = CARDNUM(i + 1))
        num_statistic[i] = 0;
    
    for (Card c : cards) {
        suit_statistic[c.getSuit()]++;
        num_statistic[c.getNum()]++;
    }
    bool flush = 0, straight = 0, quadra = 0, tag = 0;
    CARDNUM quaNum= NUM_2, straiNum= NUM_2, quaKicker;
    int pair = 0;
    CARDNUM pairNum[3];// 最多三对（不含三条及以上）
    int trible = 0;
    CARDNUM triNum[2]; // 最多两个三条 (不含)
    std::vector<CARDNUM> highNum;
    std::vector<CARDNUM> flushCards;
    // check flush
    for (auto it : suit_statistic)
        if (it.second >= 5) {
            flush = 1;
            for (Card c : cards)
                if (c.getSuit() == it.first)
                    flushCards.push_back(c.getNum());
            std::sort(flushCards.rbegin(), flushCards.rend());
            for (size_t i = 0; i <= flushCards.size() - 5; i++) {
                if (flushCards[i] == flushCards[i + 4] - 4) {
                    tag = 1;
                    straiNum = flushCards[i];
                    break;
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
            triNum[trible++] = i;
            break;
        case 2:
            pairNum[pair++] = i;
            break;
        case 1://  记录kicker
            highNum.insert(highNum.begin(), i);
            break;
        case 0://   清空顺子记录
            k = 0;
            strai_low = CARDNUM(i + 1);
            break;
        default:
            break;
        }
        if (num_statistic[i] >= 1 && i == strai_low + 1) {//    判断顺子
            strai_low = i;
            k++;
            if (k >= 4 && !tag) {
                straight = 1;
                straiNum = strai_low;
            }
        }
    }
    if (quadra) {   // 四条牌型，三条和对子的牌都可能是kicker
        quaKicker = NUM_2;
        if (highNum.size() > 0) quaKicker = highNum[0];
        if (trible && triNum[trible - 1] > quaKicker) quaKicker = triNum[trible-1];
        if (pair && pairNum[pair-1] > quaKicker) quaKicker = pairNum[pair-1];
    }
    // return result
    if (flush && straight && tag) return {STRAIGHT_FLUSH, {straiNum}};
    if (quadra) return {FOUR_OF_A_KIND, {quaNum, quaKicker}};
    if (trible >= 2) return {FULL_HOUSE, {triNum[1], triNum[0]}};
    if (trible==1 && pair) return {FULL_HOUSE, {triNum[0], pairNum[pair - 1]}};
    if (flush) return {FLUSH, flushCards};
    if (straight) return {STRAIGHT, {straiNum}};
    if (trible == 1) return {THREE_OF_A_KIND, {triNum[0], highNum[0], highNum[1]}};
    if (pair >= 2) return {TWO_PAIR, {pairNum[pair-1], pairNum[pair-2], highNum[0]}};
    if (pair == 1) {
        std::vector<CARDNUM> kt = {pairNum[0]};
        if (highNum.size() > 0)
            kt.push_back(highNum[0]);
        return {ONE_PAIR, kt};
    }
    return {HIGH_CARD, highNum};
}

int HandType::compareHandType(const HandType& t1, const HandType& t2) {
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

bool advancedHandType::load(const std::string& bin_path) {
    std::ifstream file(bin_path, std::ios::binary);
    if (!file) return false;

    flush_map.clear();
    other_map.clear();

    int cnt, val;
    uint64_t key;

    file.read(reinterpret_cast<char*>(&cnt), sizeof(cnt));
    for (int i = 0; i < cnt; i++) {
        file.read(reinterpret_cast<char*>(&key), sizeof(key));
        file.read(reinterpret_cast<char*>(&val), sizeof(val));
        flush_map[key] = val;
    }

    file.read(reinterpret_cast<char*>(&cnt), sizeof(cnt));
    for (int i = 0; i < cnt; i++) {
        file.read(reinterpret_cast<char*>(&key), sizeof(key));
        file.read(reinterpret_cast<char*>(&val), sizeof(val));
        other_map[key] = val;
    }

    file.close();
    loaded = true;
    return true;
}

void advancedHandType::convert(std::unordered_map<uint64_t, int>& strength_map) {
    flush_map.clear();
    other_map.clear();
    for (auto it = strength_map.begin(); it != strength_map.end(); ++it) {
        if (isFlush(it->first))
            flush_map[it->first] = it->second;
        else
            other_map[ranksHash(it->first)] = it->second;
    }
    loaded = true;
}

int advancedHandType::operator[](const uint64_t& hash) {
    auto it = flush_map.find(hash);
    if (it != flush_map.end()) return it->second;

    uint64_t hash_compressed = ranksHash(hash);
    auto it2 = other_map.find(hash_compressed);
    if (it2 != other_map.end()) return it2->second;

    return -1;
}

// Iterative k-combination generator: calls callback(set[i]...) for each combination
template<typename T, typename F>
void forEachCombination(const std::vector<T>& set, int k, F&& callback) {
    int n = set.size();
    if (k <= 0 || k > n) return;
    std::vector<int> idx(k);
    for (int i = 0; i < k; i++) idx[i] = i;
    while (true) {
        callback(set[idx[0]], set[idx[1]], set[idx[2]], set[idx[3]], set[idx[4]]);
        int i = k - 1;
        while (i >= 0 && idx[i] == n - k + i) i--;
        if (i < 0) break;
        idx[i]++;
        for (int j = i + 1; j < k; j++) idx[j] = idx[j - 1] + 1;
    }
}

int advancedEvaluate(const std::vector<Card>& cards) {
    if (!g_advancedRanker.loaded || cards.size() < 5) return -1;

    std::vector<int> ints;
    ints.reserve(cards.size());
    for (const auto& c : cards) ints.push_back(c.toInt());

    int best_rank = INT_MAX;
    forEachCombination(ints, 5, [&](int a, int b, int c, int d, int e) {
        uint64_t mask = (1ULL << a) | (1ULL << b) | (1ULL << c) | (1ULL << d) | (1ULL << e);
        int rank = g_advancedRanker[mask];
        if (rank >= 0 && rank < best_rank) best_rank = rank;
    });

    return best_rank == INT_MAX ? -1 : best_rank;
}

std::ostream& operator<<(std::ostream& out, const HandType& t) {
    out << t.to_string();
    return out;
}