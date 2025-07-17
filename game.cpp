#include "game.h"

void Game::init() {
    pile = new Poker[52];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 13; j++)
            pile[i*13+j] = Poker(CARDNUM(j), SUIT(i));
}


Game::Game() {
    init();


}

Game::~Game() {
    delete[] pile;
}
