#include "position.h"

void showActionMenu()
{
	std::cout << "==========================" << std::endl;
	std::cout << "=== Fold           [1] ===" << std::endl;
	std::cout << "=== Call/Check     [2] ===" << std::endl;
	std::cout << "=== Bet/Raise      [3] ===" << std::endl;
	std::cout << "=== QUIT         [Esc] ===" << std::endl;
	std::cout << "==========================" << std::endl;
}

void showPositionMenu(const Position& pos)
{
	std::cout << "==========================" << std::endl;
	for (int i = 0; i < pos.getPlayerNum(); i++) {
		std::cout << "=== " << pos[i] << " \t  [" << i << "]  ===" << std::endl;
	}
	std::cout << "=== QUIT GAME    [Esc] ===" << std::endl;
	std::cout << "==========================" << std::endl;
}
