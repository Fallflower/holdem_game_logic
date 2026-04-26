#include "deck.h"
#include <random>
#include <algorithm>
#include <sstream>

Deck::Deck() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
            pile_.push_back(Card(CARDNUM(j), SUIT(i)));
}

void Deck::shuffle() {
    std::random_device seed;
    std::ranlux48 engine(seed());
    std::shuffle(pile_.begin(), pile_.end(), engine);
}

void Deck::deal(int playerNum, std::vector<std::vector<Card>>& hands) {
    for (int i = 0; i < playerNum; i++)
        if (pile_[i] > pile_[i + playerNum])
            hands.push_back({pile_[i], pile_[i + playerNum]});
        else
            hands.push_back({pile_[i + playerNum], pile_[i]});
    int j = 2 * playerNum;
    pubCards_.assign(pile_.begin() + j, pile_.begin() + j + 5);
}

std::vector<Card> Deck::remainingDeck(int playerNum, int knownPubCards) const {
    return std::vector<Card>(
        pile_.begin() + 2 * playerNum + knownPubCards,
        pile_.end()
    );
}

std::string Deck::pubCardsStr(int stateCode) const {
    std::vector<std::string> temp = {"??", "??", "??", "??", "??"};
    switch (stateCode) {
    case 3: temp[4] = pubCards_[4].toString(); [[fallthrough]];
    case 2: temp[3] = pubCards_[3].toString(); [[fallthrough]];
    case 1:
        temp[0] = pubCards_[0].toString();
        temp[1] = pubCards_[1].toString();
        temp[2] = pubCards_[2].toString();
        break;
    default: break;
    }
    std::ostringstream oss;
    for (size_t i = 0; i < temp.size(); i++) {
        if (i > 0) oss << "  ";
        oss << temp[i];
    }
    return oss.str();
}

std::string Deck::pubCardsColStr(int stateCode) const {
    std::vector<std::string> temp = {"????", "????", "????", "????", "????"};
    switch (stateCode) {
    case 3: temp[4] = pubCards_[4].toColorString(); [[fallthrough]];
    case 2: temp[3] = pubCards_[3].toColorString(); [[fallthrough]];
    case 1:
        temp[0] = pubCards_[0].toColorString();
        temp[1] = pubCards_[1].toColorString();
        temp[2] = pubCards_[2].toColorString();
        break;
    default: break;
    }
    std::ostringstream oss;
    for (size_t i = 0; i < temp.size(); i++) {
        if (i > 0) oss << "  ";
        oss << temp[i];
    }
    return oss.str();
}

