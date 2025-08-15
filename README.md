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
int call();                     // 当前玩家跟注 (返回需加注数量)
void bet(const int& amount);    // 当前玩家下注
```

#### 游戏状态查询
```cpp
void show() const;              // 打印前游戏状态所有信息
int getPot() const;             // 获取当前底池总额
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
    // 创建6人游戏，庄家位置索引为2
    Game g(6, 2);
    // g.show();    // 显示初始状态

    g.bet(4);       // 枪口位下注4
    g.fold();       // 劫位弃牌
    g.call();       // 关位跟注
    g.fold();       // 庄位弃牌
    g.fold();       // 小盲弃牌
    g.call();       // 大盲过牌

    g.show();       // 游戏进入翻牌阶段
    // 打印赢家
    for (auto i : g.checkWinner()) {
        std::cout << i << std::endl;
    }
    return 0;
}
```
### 输出实例
（直接设置成河牌阶段进行演示）
```bash
>.\main.exe
================================================================
  Public: 
                        3♣  T♣  J♣  Q♠  Q♦
   State:  river
     Pot:  3
----------------------------------------------------------------
  Player1 ( H J ):   K♥ 2♠      0       对Q K踢
  Player2 ( C O ):   Q♣ 8♥      0       三条Q J踢
  Player3 (  D  ):   A♣ Q♥      0       三条Q A踢
  Player4 ( S B ):   J♥ 8♠      1       对Q对J T踢
  Player5 ( B B ):   3♠ 2♥      2       对Q对3 J踢
 *Player6 ( UTG ):   7♣ 2♣      0       同花J
================================================================
Winner index: 5
```

### 关键操作说明
1. **下注**:
   ```cpp
   game.bet(10); // 当前玩家下注10筹码
   ```

2. **跟注**:
   ```cpp
   int needed = game.call(); // 返回需要跟注的筹码数
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
