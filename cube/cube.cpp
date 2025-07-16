#include <graphics.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

// 游戏常量
const int BLOCK_SIZE = 30;          // 方块大小
const int BOARD_WIDTH = 10;         // 游戏区域宽度
const int BOARD_HEIGHT = 20;        // 游戏区域高度
const int PREVIEW_WIDTH = 5;        // 预览区域宽度
const int PREVIEW_HEIGHT = 5;       // 预览区域高度
const int BOARD_X = 100;            // 游戏区域左上角X坐标
const int BOARD_Y = 50;             // 游戏区域左上角Y坐标
const int PREVIEW_X = 500;          // 预览区域左上角X坐标
const int PREVIEW_Y = 100;          // 预览区域左上角Y坐标
const int WINDOW_WIDTH = 700;       // 窗口宽度
const int WINDOW_HEIGHT = 1000;      // 窗口高度

// 方块颜色
COLORREF BLOCK_COLORS[7] = {
    RGB(255, 0, 0),     // 红色 - I
    RGB(0, 255, 0),     // 绿色 - J
    RGB(0, 0, 255),     // 蓝色 - L
    RGB(255, 255, 0),   // 黄色 - O
    RGB(255, 0, 255),   // 紫色 - S
    RGB(0, 255, 255),   // 青色 - T
    RGB(255, 165, 0)    // 橙色 - Z
};

// 方块形状定义
int SHAPES[7][4][4][4] = {
    // I
    {
        {
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0},
            {0,0,1,0}
        },
        {
            {0,0,0,0},
            {0,0,0,0},
            {1,1,1,1},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,1,0,0}
        }
    },
    // J
    {
        {
            {1,0,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {1,1,0,0},
            {0,0,0,0}
        }
    },
    // L
    {
        {
            {0,0,1,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,0,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {1,0,0,0},
            {0,0,0,0}
        },
        {
            {1,1,0,0},
            {0,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        }
    },
    // O
    {
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,1,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        }
    },
    // S
    {
        {
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,0,1,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {0,1,1,0},
            {1,1,0,0},
            {0,0,0,0}
        },
        {
            {1,0,0,0},
            {1,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        }
    },
    // T
    {
        {
            {0,1,0,0},
            {1,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {1,1,0,0},
            {0,1,0,0},
            {0,0,0,0}
        }
    },
    // Z
    {
        {
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0},
            {0,0,0,0}
        },
        {
            {0,0,1,0},
            {0,1,1,0},
            {0,1,0,0},
            {0,0,0,0}
        },
        {
            {0,0,0,0},
            {1,1,0,0},
            {0,1,1,0},
            {0,0,0,0}
        },
        {
            {0,1,0,0},
            {1,1,0,0},
            {1,0,0,0},
            {0,0,0,0}
        }
    }
};

class Tetris {
private:
    int board[BOARD_HEIGHT][BOARD_WIDTH] = { 0 }; // 游戏板
    int currentBlock[4][4];                     // 当前方块
    int nextBlock[4][4];                        // 下一个方块
    int currentX, currentY;                     // 当前方块位置
    int currentType;                            // 当前方块类型
    int nextType;                               // 下一个方块类型
    int currentRotation;                        // 当前旋转状态
    int score;                                  // 当前分数
    int level;                                  // 当前等级
    int lines;                                  // 消除行数
    bool gameOver;                              // 游戏结束标志
    double fallSpeed;                           // 下落速度（秒）
    double lastFallTime;                        // 上次下落时间
    COLORREF currentColor;                      // 当前方块颜色

    // 生成随机方块
    void generateBlock(int block[4][4], int& type) {
        type = rand() % 7;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                block[i][j] = SHAPES[type][0][i][j];
            }
        }
        currentColor = BLOCK_COLORS[type];
    }

    // 检查位置是否有效
    bool isValidPosition(int x, int y, int rotation = -1) {
        int rot = (rotation == -1) ? currentRotation : rotation;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (currentBlock[i][j] == 0) continue;

                int boardX = x + j;
                int boardY = y + i;

                // 检查边界
                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT) {
                    return false;
                }

                // 检查是否与已有方块重叠
                if (boardY >= 0 && board[boardY][boardX] != 0) {
                    return false;
                }
            }
        }
        return true;
    }

public:
    Tetris() {
        srand(static_cast<unsigned int>(time(nullptr)));
        reset();
    }

    // 重置游戏
    void reset() {
        // 清空游戏板
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                board[i][j] = 0;
            }
        }

        // 重置游戏状态
        score = 0;
        level = 1;
        lines = 0;
        gameOver = false;
        fallSpeed = 0.5;
        lastFallTime = 0;

        // 生成初始方块
        generateBlock(currentBlock, currentType);
        currentRotation = 0;
        currentX = BOARD_WIDTH / 2 - 2;
        currentY = 0;

        // 生成下一个方块
        generateBlock(nextBlock, nextType);
    }

    // 旋转方块
    void rotate() {
        if (gameOver) return;

        int newRotation = (currentRotation + 1) % 4;
        int tempBlock[4][4];

        // 创建旋转后的方块
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tempBlock[i][j] = SHAPES[currentType][newRotation][i][j];
            }
        }

        // 保存原始方块
        int originalBlock[4][4];
        memcpy(originalBlock, currentBlock, sizeof(currentBlock));

        // 尝试旋转
        memcpy(currentBlock, tempBlock, sizeof(currentBlock));

        // 如果旋转后位置无效，恢复原始方块
        if (!isValidPosition(currentX, currentY)) {
            memcpy(currentBlock, originalBlock, sizeof(currentBlock));
        }
        else {
            currentRotation = newRotation;
        }
    }

    // 移动方块
    void move(int dx, int dy) {
        if (gameOver) return;

        if (isValidPosition(currentX + dx, currentY + dy)) {
            currentX += dx;
            currentY += dy;
        }
    }

    // 快速下落
    void drop() {
        if (gameOver) return;

        while (isValidPosition(currentX, currentY + 1)) {
            currentY++;
        }
        placeBlock();
    }

    // 放置方块到游戏板
    void placeBlock() {
        if (gameOver) return;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (currentBlock[i][j] == 0) continue;

                int boardX = currentX + j;
                int boardY = currentY + i;

                if (boardY < 0) {
                    // 游戏结束
                    gameOver = true;
                    return;
                }

                board[boardY][boardX] = currentType + 1;
            }
        }

        // 检查并消除完整的行
        clearLines();

        // 生成新方块
        memcpy(currentBlock, nextBlock, sizeof(currentBlock));
        currentType = nextType;
        currentRotation = 0;
        generateBlock(nextBlock, nextType);
        currentX = BOARD_WIDTH / 2 - 2;
        currentY = 0;

        // 检查游戏是否结束
        if (!isValidPosition(currentX, currentY)) {
            gameOver = true;
        }
    }

    // 消除完整的行
    void clearLines() {
        int linesCleared = 0;

        for (int i = BOARD_HEIGHT - 1; i >= 0; i--) {
            bool lineComplete = true;
            for (int j = 0; j < BOARD_WIDTH; j++) {
                if (board[i][j] == 0) {
                    lineComplete = false;
                    break;
                }
            }

            if (lineComplete) {
                linesCleared++;
                // 移动上面的行下来
                for (int k = i; k > 0; k--) {
                    for (int j = 0; j < BOARD_WIDTH; j++) {
                        board[k][j] = board[k - 1][j];
                    }
                }
                // 清空最顶行
                for (int j = 0; j < BOARD_WIDTH; j++) {
                    board[0][j] = 0;
                }
                // 重新检查当前行
                i++;
            }
        }

        // 更新分数
        if (linesCleared > 0) {
            lines += linesCleared;
            // 根据消除的行数计算分数
            switch (linesCleared) {
            case 1: score += 100 * level; break;
            case 2: score += 300 * level; break;
            case 3: score += 500 * level; break;
            case 4: score += 800 * level; break;
            }

            // 更新等级和速度
            level = lines / 10 + 1;
            fallSpeed = 0.5 - (level - 1) * 0.05;
            if (fallSpeed < 0.1) fallSpeed = 0.1;
        }
    }

    // 更新游戏状态
    void update(double currentTime) {
        if (gameOver) return;

        // 自动下落
        if (currentTime - lastFallTime > fallSpeed) {
            if (isValidPosition(currentX, currentY + 1)) {
                currentY++;
            }
            else {
                placeBlock();
            }
            lastFallTime = currentTime;
        }
    }

    // 绘制游戏
    void draw() {
        // 绘制背景
        setfillcolor(RGB(30, 30, 50));
        solidrectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        // 绘制游戏区域边框
        setfillcolor(RGB(20, 20, 40));
        solidrectangle(BOARD_X - 10, BOARD_Y - 10,
            BOARD_X + BOARD_WIDTH * BLOCK_SIZE + 10,
            BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 10);

        // 绘制游戏区域背景
        setfillcolor(RGB(10, 10, 20));
        solidrectangle(BOARD_X, BOARD_Y,
            BOARD_X + BOARD_WIDTH * BLOCK_SIZE,
            BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE);

        // 绘制网格
        setlinecolor(RGB(40, 40, 60));
        for (int i = 0; i <= BOARD_HEIGHT; i++) {
            line(BOARD_X, BOARD_Y + i * BLOCK_SIZE,
                BOARD_X + BOARD_WIDTH * BLOCK_SIZE,
                BOARD_Y + i * BLOCK_SIZE);
        }
        for (int j = 0; j <= BOARD_WIDTH; j++) {
            line(BOARD_X + j * BLOCK_SIZE, BOARD_Y,
                BOARD_X + j * BLOCK_SIZE,
                BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE);
        }

        // 绘制已落下的方块
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                if (board[i][j] != 0) {
                    int type = board[i][j] - 1;
                    setfillcolor(BLOCK_COLORS[type]);
                    solidrectangle(BOARD_X + j * BLOCK_SIZE + 1,
                        BOARD_Y + i * BLOCK_SIZE + 1,
                        BOARD_X + (j + 1) * BLOCK_SIZE - 1,
                        BOARD_Y + (i + 1) * BLOCK_SIZE - 1);

                    // 绘制方块内部高光
                    setfillcolor(RGB(255, 255, 255, 100));
                    solidrectangle(BOARD_X + j * BLOCK_SIZE + 1,
                        BOARD_Y + i * BLOCK_SIZE + 1,
                        BOARD_X + j * BLOCK_SIZE + BLOCK_SIZE / 2,
                        BOARD_Y + i * BLOCK_SIZE + BLOCK_SIZE / 2);
                }
            }
        }

        // 绘制当前方块
        if (!gameOver) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (currentBlock[i][j] != 0) {
                        int x = currentX + j;
                        int y = currentY + i;
                        if (y >= 0) { // 只绘制在游戏区域内的部分
                            setfillcolor(currentColor);
                            solidrectangle(BOARD_X + x * BLOCK_SIZE + 1,
                                BOARD_Y + y * BLOCK_SIZE + 1,
                                BOARD_X + (x + 1) * BLOCK_SIZE - 1,
                                BOARD_Y + (y + 1) * BLOCK_SIZE - 1);

                            // 绘制方块内部高光
                            setfillcolor(RGB(255, 255, 255, 100));
                            solidrectangle(BOARD_X + x * BLOCK_SIZE + 1,
                                BOARD_Y + y * BLOCK_SIZE + 1,
                                BOARD_X + x * BLOCK_SIZE + BLOCK_SIZE / 2,
                                BOARD_Y + y * BLOCK_SIZE + BLOCK_SIZE / 2);
                        }
                    }
                }
            }
        }

        // 绘制预览区域边框
        setfillcolor(RGB(20, 20, 40));
        solidrectangle(PREVIEW_X - 10, PREVIEW_Y - 10,
            PREVIEW_X + PREVIEW_WIDTH * BLOCK_SIZE + 10,
            PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE + 10);

        // 绘制预览区域背景
        setfillcolor(RGB(10, 10, 20));
        solidrectangle(PREVIEW_X, PREVIEW_Y,
            PREVIEW_X + PREVIEW_WIDTH * BLOCK_SIZE,
            PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE);

        // 绘制预览文字
        settextcolor(RGB(200, 200, 255));
        settextstyle(24, 0, _T("Arial"));
        outtextxy(PREVIEW_X, PREVIEW_Y - 40, _T("下一个方块"));

        // 绘制下一个方块
        int centerX = PREVIEW_WIDTH / 2 - 2;
        int centerY = PREVIEW_HEIGHT / 2 - 2;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (nextBlock[i][j] != 0) {
                    setfillcolor(BLOCK_COLORS[nextType]);
                    solidrectangle(PREVIEW_X + (centerX + j) * BLOCK_SIZE + 1,
                        PREVIEW_Y + (centerY + i) * BLOCK_SIZE + 1,
                        PREVIEW_X + (centerX + j + 1) * BLOCK_SIZE - 1,
                        PREVIEW_Y + (centerY + i + 1) * BLOCK_SIZE - 1);

                    // 绘制方块内部高光
                    setfillcolor(RGB(255, 255, 255, 100));
                    solidrectangle(PREVIEW_X + (centerX + j) * BLOCK_SIZE + 1,
                        PREVIEW_Y + (centerY + i) * BLOCK_SIZE + 1,
                        PREVIEW_X + (centerX + j) * BLOCK_SIZE + BLOCK_SIZE / 2,
                        PREVIEW_Y + (centerY + i) * BLOCK_SIZE + BLOCK_SIZE / 2);
                }
            }
        }

        // 绘制分数板
        setfillcolor(RGB(20, 20, 40));
        solidrectangle(PREVIEW_X - 10, PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE + 30,
            PREVIEW_X + PREVIEW_WIDTH * BLOCK_SIZE + 10,
            PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE + 200);

        settextcolor(RGB(200, 200, 255));
        settextstyle(20, 0, _T("Arial"));

        TCHAR scoreStr[50];
        swprintf_s(scoreStr, _T("分数: %d"), score);
        outtextxy(PREVIEW_X, PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE + 50, scoreStr);

        TCHAR levelStr[50];
        swprintf_s(levelStr, _T("等级: %d"), level);
        outtextxy(PREVIEW_X, PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE + 80, levelStr);

        TCHAR linesStr[50];
        swprintf_s(linesStr, _T("行数: %d"), lines);
        outtextxy(PREVIEW_X, PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE + 110, linesStr);

        // 绘制操作说明
        settextcolor(RGB(150, 150, 200));
        settextstyle(16, 0, _T("Arial"));
        outtextxy(BOARD_X, BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 20, _T("← → : 左右移动"));
        outtextxy(BOARD_X, BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 45, _T("↑ : 旋转"));
        outtextxy(BOARD_X, BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 70, _T("↓ : 加速下落"));
        outtextxy(BOARD_X, BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 95, _T("空格 : 直接落下"));
        outtextxy(BOARD_X, BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 120, _T("R : 重新开始"));
        outtextxy(BOARD_X, BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 145, _T("ESC : 退出"));

        // 绘制游戏结束提示
        if (gameOver) {
            settextcolor(RGB(255, 50, 50));
            settextstyle(36, 0, _T("Arial"));
            outtextxy(BOARD_X + BOARD_WIDTH * BLOCK_SIZE / 2 - 100,
                BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE / 2 - 50,
                _T("游戏结束!"));

            settextstyle(24, 0, _T("Arial"));
            outtextxy(BOARD_X + BOARD_WIDTH * BLOCK_SIZE / 2 - 80,
                BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE / 2 + 10,
                _T("按R键重新开始"));
        }

        // 绘制标题
        settextcolor(RGB(100, 200, 255));
        settextstyle(48, 0, _T("Arial"));
        outtextxy(WINDOW_WIDTH / 2 - 120, 10, _T("俄罗斯方块"));
    }

    bool isGameOver() const { return gameOver; }
};

int main() {
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    setbkcolor(BLACK);
    cleardevice();

    // 启用双缓冲
    BeginBatchDraw();

    Tetris game;
    double lastTime = GetTickCount() / 1000.0;

    while (true) {
        // 计算时间增量
        double currentTime = GetTickCount() / 1000.0;
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // 处理键盘输入
        if (_kbhit()) {
            int key = _getch();
            switch (key) {
            case 'a':
            case 'A':
            case 75: // 左箭头
                game.move(-1, 0);
                break;
            case 'd':
            case 'D':
            case 77: // 右箭头
                game.move(1, 0);
                break;
            case 's':
            case 'S':
            case 80: // 下箭头
                game.move(0, 1);
                break;
            case 'w':
            case 'W':
            case 72: // 上箭头
                game.rotate();
                break;
            case ' ': // 空格
                game.drop();
                break;
            case 'r':
            case 'R': // 重新开始
                game.reset();
                break;
            case 27: // ESC
                return 0;
            }
        }

        // 更新游戏状态
        game.update(currentTime);

        // 绘制游戏
        cleardevice();
        game.draw();

        // 刷新显示
        FlushBatchDraw();

        // 控制帧率
        Sleep(10);
    }

    EndBatchDraw();
    closegraph();
    return 0;
}