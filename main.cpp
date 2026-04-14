#include "game.h"
#include "assistant.h"
#include "cliMenu.h"

int main() {
    setConsoleToUTF8();
    std::cout << "========================Welcome !===============================" << std::endl;
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
            std::cin >> n;
            g.bet(n); break;
        case 27:  // [Esc]
            return 1;
        default:
            break;
        }
    }

    std::cout << "Winner: ";
    for (auto i : g.checkWinner()) {
        std::cout << "Player" << i + 1 << "; ";
    }
    std::cout << std::endl;
    return 0;
}