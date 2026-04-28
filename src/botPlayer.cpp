#include "botPlayer.h"
#include <cstdlib>

ACTION BotPlayer::makeAction(const int& chipsToCall, int &betAmount) {
    int r = rand() % 100;
    int pot = betAmount;    // betAmount先传入底池大小，后返回玩家下注金额

    if (chipsToCall == 0) {
        if (r < 70) {
            return CHECK;
        } else if (r < 90) {
            betAmount = pot / 3;
            return BET;
        } else {
            betAmount = pot;
            setChips(0);
            if (betAmount >= chips) {
                betAmount = chips;
                setChips(0);
                return ALLIN;
            }
            return BET;
        }
    } else if (chipsToCall < chips) {
        if (r < 60) {
            decChips(chipsToCall);
            return CALL;
        } else if (r < 85) {
            return FOLD;
        } else {
            betAmount = chipsToCall * 3;
            if (betAmount >= chips) {
                betAmount = chips;
                setChips(0);
                return ALLIN;
            }
            return RAISE;
        }
    } else {
        if (r < 50) {
            betAmount = chips;
            setChips(0);
            return ALLIN;
        } else {
            return FOLD;
        }
    }
}
