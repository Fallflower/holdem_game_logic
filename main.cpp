#include "game.h"

int main() {
    Game g(6, 2);
    g.show();

    g.bet(4);
    g.fold();
    g.call();
    g.fold();
    g.fold();
    g.call();
    // std::cout << g.call() << std::endl;
    g.show();

    // while (1)
    // {
    //     int b;
    //     std::cout << "To bet: " << std::flush;
    //     std::cin >> b;
    //     g.bet(b);
    //     g.show();
    // }
    

    // test
    // Position p(9, 1);
    // std::cout << p << std::endl;
    // Game g(9, 2);
    // std::cout << g.getPot() << std::endl;
    
    return 0;
}