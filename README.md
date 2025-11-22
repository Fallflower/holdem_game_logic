# 德州扑克游戏逻辑处理模块 - C++实现

## 项目概述
本项目实现了一个德州扑克游戏的核心逻辑处理类`Game`，使用C++编写。该类封装了德州扑克游戏的全套流程控制逻辑，包括发牌、下注轮次管理、牌型判断、赢家判定等关键功能，提供给前端项目调用。

## 类设计说明

### 主要成员变量
```cpp
int playerNum;              // 玩家数量
int stateCode;              // 游戏状态 (0:Pre-flop, 1:Flop, 2:Turn, 3:River)
int dealer;                 // 庄家位置索引
int active;                 // 当前行动玩家索引
int lastBet;                // 最后下注玩家索引
int *chips;                 // 玩家已投入的筹码
bool *ftag;                 // 玩家弃牌标记数组
```

### 主要成员函数

#### 玩家操作
```cpp
void fold();                    // 当前玩家弃牌
void call();                    // 当前玩家跟注
void bet(const int& amount);    // 当前玩家下注
```

#### 游戏状态查询
```cpp
void show() const;              // 打印前游戏状态所有信息
int getPot() const;             // 获取当前底池总额
int getChipsToCall() const;     // 返回需跟注注数量
int getState() const;           // 获取当前游戏状态
```

#### 胜负判定
```cpp
std::vector<int> checkWinner() const; // 检查赢家(返回赢家索引数组)
```

### 游戏状态说明
游戏通过`stateCode`管理状态流转：
- `0`: Pre-flop (翻牌前)
- `1`: Flop (翻牌圈)
- `2`: Turn (转牌圈)
- `3`: River (河牌圈)

## 使用示例

### 基本游戏流程（请参考main.cpp）
```cpp
#include "game.h"

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
        case '3':
            int n;
            std::cin >> n;
            g.bet(n); break;
        case 27:
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
```
### 输出实例
```bash
>.\main.exe
================================================================
  Public: 
                        ??  ??  ??  ??  ??
   State:  preflop
     Pot:  3
----------------------------------------------------------------
  Player1 ( H J ):   8♠ 6♥      0       胜率: 12.60%    高牌8
  Player2 ( C O ):   J♣ 3♥      0       胜率: 13.02%    高牌J
  Player3 (  D  ):   Q♦ 6♣      0       胜率: 1.80%     高牌Q
  Player4 ( S B ):   K♦ 4♣      1       胜率: 7.83%     高牌K
  Player5 ( B B ):   K♠ 4♥      2       胜率: 8.09%     高牌K
 *Player6 ( UTG ):   Q♥ Q♠      0       胜率: 20.88%    对Q
  Player7 (UTG+1):   9♣ 9♠      0       胜率: 23.39%    对9
  Player8 (UTG+2):   Q♣ 2♦      0       胜率: 2.10%     高牌Q
  Player9 ( M P ):   5♦ 2♣      0       胜率: 10.30%    高牌5
================================================================
==========================
=== Fold           [1] ===
=== Call/Check     [2] ===
=== Bet/Raise      [3] ===
=== QUIT         [Esc] ===
==========================
Please Choose:{1 2 3 [Esc]}: 
......(after many operations)
================================================================
  Public:
                        3♣  T♥  7♥  ??  ??
   State:  flop
     Pot:  39
----------------------------------------------------------------
  Player1 ( H J ):   8♠ 6♥      0       (fold)          高牌T
  Player2 ( C O ):   J♣ 3♥      0       (fold)          对3 J踢
  Player3 (  D  ):   Q♦ 6♣      0       (fold)          高牌Q
  Player4 ( S B ):   K♦ 4♣      0       (fold)          高牌K
  Player5 ( B B ):   K♠ 4♥      0       (fold)          高牌K
  Player6 ( UTG ):   Q♥ Q♠      0       胜率: 85.61%    对Q T踢
 *Player7 (UTG+1):   9♣ 9♠      0       胜率: 14.39%    对9 T踢
  Player8 (UTG+2):   Q♣ 2♦      0       (fold)          高牌Q
  Player9 ( M P ):   5♦ 2♣      0       (fold)          高牌T
================================================================
==========================
=== Fold           [1] ===
=== Call/Check     [2] ===
=== Bet/Raise      [3] ===
=== QUIT         [Esc] ===
==========================
Please Choose:{1 2 3 [Esc]}:
......(after many operations)
================================================================
  Public:
                        3♣  T♥  7♥  8♥  ??
   State:  turn
     Pot:  57
----------------------------------------------------------------
  Player1 ( H J ):   8♠ 6♥      0       (fold)          对8 T踢
  Player2 ( C O ):   J♣ 3♥      0       (fold)          对3 J踢
  Player3 (  D  ):   Q♦ 6♣      0       (fold)          高牌Q
  Player4 ( S B ):   K♦ 4♣      0       (fold)          高牌K
  Player5 ( B B ):   K♠ 4♥      0       (fold)          高牌K
  Player6 ( UTG ):   Q♥ Q♠      18      胜率: 83.46%    对Q T踢
 *Player7 (UTG+1):   9♣ 9♠      0       胜率: 16.54%    对9 T踢
  Player8 (UTG+2):   Q♣ 2♦      0       (fold)          高牌Q
  Player9 ( M P ):   5♦ 2♣      0       (fold)          高牌T
================================================================
==========================
=== Fold           [1] ===
=== Call/Check     [2] ===
=== Bet/Raise      [3] ===
=== QUIT         [Esc] ===
==========================
Please Choose:{1 2 3 [Esc]}:
......(after some operations)
================================================================
  Public:
                        3♣  T♥  7♥  8♥  7♣
   State:  river
     Pot:  135
----------------------------------------------------------------
  Player1 ( H J ):   8♠ 6♥      0       (fold)          对8对7 T踢
  Player2 ( C O ):   J♣ 3♥      0       (fold)          对7对3 J踢
  Player3 (  D  ):   Q♦ 6♣      0       (fold)          对7 Q踢
  Player4 ( S B ):   K♦ 4♣      0       (fold)          对7 K踢
  Player5 ( B B ):   K♠ 4♥      0       (fold)          对7 K踢
  Player6 ( UTG ):   Q♥ Q♠      60      胜率: 100.00%   对Q对7 T踢
 *Player7 (UTG+1):   9♣ 9♠      0       胜率: 0.00%     对9对7 T踢
  Player8 (UTG+2):   Q♣ 2♦      0       (fold)          对7 Q踢
  Player9 ( M P ):   5♦ 2♣      0       (fold)          对7 T踢
================================================================
==========================
=== Fold           [1] ===
=== Call/Check     [2] ===
=== Bet/Raise      [3] ===
=== QUIT         [Esc] ===
==========================
Please Choose:{1 2 3 [Esc]}:
Winner: Player6; 
```

### 关键操作说明
1. **下注**:
   ```cpp
   game.bet(10); // 当前玩家下注10筹码
   ```

2. **跟注**:
   ```cpp
   int needed = game.getChipsToCall(); // 返回需要跟注的筹码数
   game.call()
   ```

3. **弃牌**:
   ```cpp
   game.fold(); // 当前玩家弃牌
   ```


## 依赖项
1. `handType.h`: 牌型判断逻辑
2. `position.h`: 玩家位置管理
3. `poker.h`: 扑克牌类
4. C++17 Standard
5. STL组件: `<vector>`, `<random>`, `<map>`等
