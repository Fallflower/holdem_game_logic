#include "game.h"
#include "assistant.h"
#include "cliMenu.h"

extern advancedHandType g_advancedRanker;

int main() {
    setConsoleToUTF8();

    g_advancedRanker.load("./resources/card5_dic_zipped.bin");

    std::cout << "Welcome to Holdem Game with GTO botplayers" << std::endl;
    std::cout << " Player Name:" << std::flush;
    std::string name;
    std::cin >> name;
    std::cout << " Players Num:" << std::flush;
    int pn;
    std::cin >> pn;
    std::cout << " Initial Chips:" << std::flush;
    int chips;
    std::cin >> chips;
    Position pos(pn, 0);
    showPositionMenu(pos);
    int hppi;
    std::cin >> hppi;

    Game g(pos, chips, Player(name, chips), hppi);
    while (!g.isEnd()) {
        clearScreen();
        g.show();
        showActionMenu();
        int k;
        k = Choice("Please Choose:", "123\x1b");
        switch (k)
        {
        case '1':
            g.fold(); break;
        case '2':
            g.call(); break;
        case '3':
            int n;
            std::cout << "Bet Amount: " << std::flush;
            std::cin >> n;
            g.bet(n); break;
        case 27:  // [Esc]
            return 0;
        default:
            break;
        }
    }

    std::cout << "Winner: ";
    for (auto i : g.checkWinner()) {
        std::cout << g.getPlayer(i).getName() << "; ";
    }
    std::cout << std::endl;
    return 0;
}