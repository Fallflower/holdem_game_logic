#include "game.h"
#include<conio.h>

int Pos(char ch, const char* str)			// 返回指定字符ch在字符串str中的下标。不存在时返回-1
{
	int i;
	for (i = 0; str[i] != '\0'; i++)
		if (ch == str[i])
			return i;
	return -1;
}

int Choice(const char* prompt, const char* options)		// 函数定义。输出提示信息prompt，输入选择的字符并返回。
{
	int key;
	std::cout << prompt << "{";
	for (int i = 0; options[i] != '\0'; i++)
	{
		if (' ' < (options[i] & 0x7f) && (options[i] & 0x7f) < 127)	// 选项options中的可打印字符
			std::cout << options[i] << ' ';
		else														// 选项options中的部分控制字符
		{
			switch (options[i])
			{
			case '\t':   std::cout << "[Tab] ";   break;					// 选项中的Tab字符'\t'（即ASCII编码9）
			case '\x1b': std::cout << "[Esc] ";   break;					// 选项中的Esc字符'\x1b'（即ASCII编码27）
			case ' ':    std::cout << "[Space] "; break;					// 选项中的空格字符' '（即ASCII编码32）
			}
		}
	}
	std::cout << "\b}: ";					// '\b'退格。先退一格，在输出右花括号
	do
	{
		key = _getch();
	} while (options[0] != '\0' && Pos(key, options) < 0); // 若options为空长度为0的字符串，则输入无限制；否则输入项应该在optins中。
	std::cout << std::endl;
	return key;
}

void showMenu()
{
	std::cout << "==========================" << std::endl;
	std::cout << "=== Fold           [1] ===" << std::endl;
	std::cout << "=== Call/Check     [2] ===" << std::endl;
	std::cout << "=== Bet/Raise      [3] ===" << std::endl;
	std::cout << "=== QUIT         [Esc] ===" << std::endl;
	std::cout << "==========================" << std::endl;
}


int main() {
    Game g(9, 2);
    while (!g.isEnd()) {
        g.show();
        showMenu();
        int k;
        k = Choice("Please Choose:", "123\x1b");
        switch (k)
        {
        case '1':
            g.fold(); break;
        case '2':
            g.call(); break;
        case 27:
            return 1;
        // case 3:
            // g.bet(); break;
        
        default:
            break;
        }
        
    }

    std::cout << "Winner: ";
    for (auto i : g.checkWinner()) {
        std::cout << "Player" << i + 1 << "; ";
    }
    std::cout << std::endl;
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