#include "botPlayer.h"
#include <thread>
#include <cstdlib>

ACTION BotPlayer::makeAction(const int& chipsToCall, int &betAmount) {
    std::cout << name << " is thinking..." << std::endl;
    // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    int r = rand() % 100;
    int pot = betAmount;    // betAmount先传入底池大小，后返回玩家下注金额

    if (chipsToCall == 0) {
        if (r < 70) {
            return CHECK;
        } else if (r < 90) {
            betAmount = pot / 3;
            decChips(betAmount);
            return BET;
        } else {
            betAmount = pot;
            if (betAmount >= chips) {
                betAmount = chips;
                setChips(0);
                return ALLIN;
            }
            decChips(betAmount);
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
            decChips(betAmount);
            return RAISE;
        }
    } else {
        if (r < 50) {
            betAmount = chips;
            setChips(0);
            return ALLINTOCALL;
        } else {
            return FOLD;
        }
    }
}
