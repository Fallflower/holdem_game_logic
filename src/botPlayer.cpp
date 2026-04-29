#include "botPlayer.h"
#include <algorithm>
#include <cstdlib>

ACTION BotPlayer::makeAction(const int& chipsToCall, int &betAmount) {
    int pot = betAmount;                    // 传入时是底池大小
    double potOdds = (pot == 0) ? 0 : (double)chipsToCall / (pot + chipsToCall);
    double r = (double)rand() / RAND_MAX;

    if (equity > 0.6) {
        // 强牌: 80%加注, 20%跟注
        if (r < 0.8) {
            betAmount = std::max(chipsToCall + 1, (int)(pot * 0.75));
            return RAISE;
        }
        return CALL;
    }

    if (equity > potOdds + 0.05) {
        // 有利可图: 跟注为主, 15%半诈唬
        if (r < 0.15) {
            betAmount = std::max(chipsToCall + 1, (int)(pot * 0.5));
            return RAISE;
        }
        return CALL;
    }

    // 无人下注时偶尔诈唬
    if (r < 0.05 && chipsToCall == 0) {
        betAmount = std::max(1, (int)(pot * 0.5));
        return RAISE;
    }

    return FOLD;
}
