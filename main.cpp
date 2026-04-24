#include "game.h"
#include "assistant.h"
#include "cliMenu.h"
#include <cstdlib>

extern advancedHandType g_advancedRanker;

int main(int argc, char* argv[]) {
    setConsoleToUTF8();

    g_advancedRanker.load("./resources/card5_dic_zipped.bin");

    std::string name = (argc > 1) ? argv[1] : "";
    int pn = (argc > 2) ? std::atoi(argv[2]) : 0;
    int chips = (argc > 3) ? std::atoi(argv[3]) : 0;
    int hppi = (argc > 4) ? std::atoi(argv[4]) : -1;

    if (name.empty()) {
        std::cout << " Player Name:" << std::flush;
        std::cin >> name;
    }
    if (!pn) {
        std::cout << " Players Num:" << std::flush;
        std::cin >> pn;
    }
    if (!chips) {
        std::cout << " Initial Chips:" << std::flush;
        std::cin >> chips;
    }
    Position pos(pn, 0);
    showPositionMenu(pos);
    if (hppi < 0) {
        std::cin >> hppi;
    }

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