#include <graphics.h>
#include <conio.h>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

// ��Ϸ����
const int BLOCK_SIZE = 30;          // �����С
const int BOARD_WIDTH = 10;         // ��Ϸ������
const int BOARD_HEIGHT = 20;        // ��Ϸ����߶�
const int PREVIEW_WIDTH = 5;        // Ԥ��������
const int PREVIEW_HEIGHT = 5;       // Ԥ������߶�
const int BOARD_X = 100;            // ��Ϸ�������Ͻ�X����
const int BOARD_Y = 50;             // ��Ϸ�������Ͻ�Y����
const int PREVIEW_X = 500;          // Ԥ���������Ͻ�X����
const int PREVIEW_Y = 100;          // Ԥ���������Ͻ�Y����
const int WINDOW_WIDTH = 700;       // ���ڿ��
const int WINDOW_HEIGHT = 1000;      // ���ڸ߶�

// ������ɫ
COLORREF BLOCK_COLORS[7] = {
    RGB(255, 0, 0),     // ��ɫ - I
    RGB(0, 255, 0),     // ��ɫ - J
    RGB(0, 0, 255),     // ��ɫ - L
    RGB(255, 255, 0),   // ��ɫ - O
    RGB(255, 0, 255),   // ��ɫ - S
    RGB(0, 255, 255),   // ��ɫ - T
    RGB(255, 165, 0)    // ��ɫ - Z
};

// ������״����
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
    int board[BOARD_HEIGHT][BOARD_WIDTH] = { 0 }; // ��Ϸ��
    int currentBlock[4][4];                     // ��ǰ����
    int nextBlock[4][4];                        // ��һ������
    int currentX, currentY;                     // ��ǰ����λ��
    int currentType;                            // ��ǰ��������
    int nextType;                               // ��һ����������
    int currentRotation;                        // ��ǰ��ת״̬
    int score;                                  // ��ǰ����
    int level;                                  // ��ǰ�ȼ�
    int lines;                                  // ��������
    bool gameOver;                              // ��Ϸ������־
    double fallSpeed;                           // �����ٶȣ��룩
    double lastFallTime;                        // �ϴ�����ʱ��
    COLORREF currentColor;                      // ��ǰ������ɫ

    // �����������
    void generateBlock(int block[4][4], int& type) {
        type = rand() % 7;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                block[i][j] = SHAPES[type][0][i][j];
            }
        }
        currentColor = BLOCK_COLORS[type];
    }

    // ���λ���Ƿ���Ч
    bool isValidPosition(int x, int y, int rotation = -1) {
        int rot = (rotation == -1) ? currentRotation : rotation;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (currentBlock[i][j] == 0) continue;

                int boardX = x + j;
                int boardY = y + i;

                // ���߽�
                if (boardX < 0 || boardX >= BOARD_WIDTH || boardY >= BOARD_HEIGHT) {
                    return false;
                }

                // ����Ƿ������з����ص�
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

    // ������Ϸ
    void reset() {
        // �����Ϸ��
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                board[i][j] = 0;
            }
        }

        // ������Ϸ״̬
        score = 0;
        level = 1;
        lines = 0;
        gameOver = false;
        fallSpeed = 0.5;
        lastFallTime = 0;

        // ���ɳ�ʼ����
        generateBlock(currentBlock, currentType);
        currentRotation = 0;
        currentX = BOARD_WIDTH / 2 - 2;
        currentY = 0;

        // ������һ������
        generateBlock(nextBlock, nextType);
    }

    // ��ת����
    void rotate() {
        if (gameOver) return;

        int newRotation = (currentRotation + 1) % 4;
        int tempBlock[4][4];

        // ������ת��ķ���
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                tempBlock[i][j] = SHAPES[currentType][newRotation][i][j];
            }
        }

        // ����ԭʼ����
        int originalBlock[4][4];
        memcpy(originalBlock, currentBlock, sizeof(currentBlock));

        // ������ת
        memcpy(currentBlock, tempBlock, sizeof(currentBlock));

        // �����ת��λ����Ч���ָ�ԭʼ����
        if (!isValidPosition(currentX, currentY)) {
            memcpy(currentBlock, originalBlock, sizeof(currentBlock));
        }
        else {
            currentRotation = newRotation;
        }
    }

    // �ƶ�����
    void move(int dx, int dy) {
        if (gameOver) return;

        if (isValidPosition(currentX + dx, currentY + dy)) {
            currentX += dx;
            currentY += dy;
        }
    }

    // ��������
    void drop() {
        if (gameOver) return;

        while (isValidPosition(currentX, currentY + 1)) {
            currentY++;
        }
        placeBlock();
    }

    // ���÷��鵽��Ϸ��
    void placeBlock() {
        if (gameOver) return;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (currentBlock[i][j] == 0) continue;

                int boardX = currentX + j;
                int boardY = currentY + i;

                if (boardY < 0) {
                    // ��Ϸ����
                    gameOver = true;
                    return;
                }

                board[boardY][boardX] = currentType + 1;
            }
        }

        // ��鲢������������
        clearLines();

        // �����·���
        memcpy(currentBlock, nextBlock, sizeof(currentBlock));
        currentType = nextType;
        currentRotation = 0;
        generateBlock(nextBlock, nextType);
        currentX = BOARD_WIDTH / 2 - 2;
        currentY = 0;

        // �����Ϸ�Ƿ����
        if (!isValidPosition(currentX, currentY)) {
            gameOver = true;
        }
    }

    // ������������
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
                // �ƶ������������
                for (int k = i; k > 0; k--) {
                    for (int j = 0; j < BOARD_WIDTH; j++) {
                        board[k][j] = board[k - 1][j];
                    }
                }
                // ������
                for (int j = 0; j < BOARD_WIDTH; j++) {
                    board[0][j] = 0;
                }
                // ���¼�鵱ǰ��
                i++;
            }
        }

        // ���·���
        if (linesCleared > 0) {
            lines += linesCleared;
            // ���������������������
            switch (linesCleared) {
            case 1: score += 100 * level; break;
            case 2: score += 300 * level; break;
            case 3: score += 500 * level; break;
            case 4: score += 800 * level; break;
            }

            // ���µȼ����ٶ�
            level = lines / 10 + 1;
            fallSpeed = 0.5 - (level - 1) * 0.05;
            if (fallSpeed < 0.1) fallSpeed = 0.1;
        }
    }

    // ������Ϸ״̬
    void update(double currentTime) {
        if (gameOver) return;

        // �Զ�����
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

    // ������Ϸ
    void draw() {
        // ���Ʊ���
        setfillcolor(RGB(30, 30, 50));
        solidrectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        // ������Ϸ����߿�
        setfillcolor(RGB(20, 20, 40));
        solidrectangle(BOARD_X - 10, BOARD_Y - 10,
            BOARD_X + BOARD_WIDTH * BLOCK_SIZE + 10,
            BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 10);

        // ������Ϸ���򱳾�
        setfillcolor(RGB(10, 10, 20));
        solidrectangle(BOARD_X, BOARD_Y,
            BOARD_X + BOARD_WIDTH * BLOCK_SIZE,
            BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE);

        // ��������
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

        // ���������µķ���
        for (int i = 0; i < BOARD_HEIGHT; i++) {
            for (int j = 0; j < BOARD_WIDTH; j++) {
                if (board[i][j] != 0) {
                    int type = board[i][j] - 1;
                    setfillcolor(BLOCK_COLORS[type]);
                    solidrectangle(BOARD_X + j * BLOCK_SIZE + 1,
                        BOARD_Y + i * BLOCK_SIZE + 1,
                        BOARD_X + (j + 1) * BLOCK_SIZE - 1,
                        BOARD_Y + (i + 1) * BLOCK_SIZE - 1);

                    // ���Ʒ����ڲ��߹�
                    setfillcolor(RGB(255, 255, 255, 100));
                    solidrectangle(BOARD_X + j * BLOCK_SIZE + 1,
                        BOARD_Y + i * BLOCK_SIZE + 1,
                        BOARD_X + j * BLOCK_SIZE + BLOCK_SIZE / 2,
                        BOARD_Y + i * BLOCK_SIZE + BLOCK_SIZE / 2);
                }
            }
        }

        // ���Ƶ�ǰ����
        if (!gameOver) {
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (currentBlock[i][j] != 0) {
                        int x = currentX + j;
                        int y = currentY + i;
                        if (y >= 0) { // ֻ��������Ϸ�����ڵĲ���
                            setfillcolor(currentColor);
                            solidrectangle(BOARD_X + x * BLOCK_SIZE + 1,
                                BOARD_Y + y * BLOCK_SIZE + 1,
                                BOARD_X + (x + 1) * BLOCK_SIZE - 1,
                                BOARD_Y + (y + 1) * BLOCK_SIZE - 1);

                            // ���Ʒ����ڲ��߹�
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

        // ����Ԥ������߿�
        setfillcolor(RGB(20, 20, 40));
        solidrectangle(PREVIEW_X - 10, PREVIEW_Y - 10,
            PREVIEW_X + PREVIEW_WIDTH * BLOCK_SIZE + 10,
            PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE + 10);

        // ����Ԥ�����򱳾�
        setfillcolor(RGB(10, 10, 20));
        solidrectangle(PREVIEW_X, PREVIEW_Y,
            PREVIEW_X + PREVIEW_WIDTH * BLOCK_SIZE,
            PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE);

        // ����Ԥ������
        settextcolor(RGB(200, 200, 255));
        settextstyle(24, 0, _T("Arial"));
        outtextxy(PREVIEW_X, PREVIEW_Y - 40, _T("��һ������"));

        // ������һ������
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

                    // ���Ʒ����ڲ��߹�
                    setfillcolor(RGB(255, 255, 255, 100));
                    solidrectangle(PREVIEW_X + (centerX + j) * BLOCK_SIZE + 1,
                        PREVIEW_Y + (centerY + i) * BLOCK_SIZE + 1,
                        PREVIEW_X + (centerX + j) * BLOCK_SIZE + BLOCK_SIZE / 2,
                        PREVIEW_Y + (centerY + i) * BLOCK_SIZE + BLOCK_SIZE / 2);
                }
            }
        }

        // ���Ʒ�����
        setfillcolor(RGB(20, 20, 40));
        solidrectangle(PREVIEW_X - 10, PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE + 30,
            PREVIEW_X + PREVIEW_WIDTH * BLOCK_SIZE + 10,
            PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE + 200);

        settextcolor(RGB(200, 200, 255));
        settextstyle(20, 0, _T("Arial"));

        TCHAR scoreStr[50];
        swprintf_s(scoreStr, _T("����: %d"), score);
        outtextxy(PREVIEW_X, PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE + 50, scoreStr);

        TCHAR levelStr[50];
        swprintf_s(levelStr, _T("�ȼ�: %d"), level);
        outtextxy(PREVIEW_X, PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE + 80, levelStr);

        TCHAR linesStr[50];
        swprintf_s(linesStr, _T("����: %d"), lines);
        outtextxy(PREVIEW_X, PREVIEW_Y + PREVIEW_HEIGHT * BLOCK_SIZE + 110, linesStr);

        // ���Ʋ���˵��
        settextcolor(RGB(150, 150, 200));
        settextstyle(16, 0, _T("Arial"));
        outtextxy(BOARD_X, BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 20, _T("�� �� : �����ƶ�"));
        outtextxy(BOARD_X, BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 45, _T("�� : ��ת"));
        outtextxy(BOARD_X, BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 70, _T("�� : ��������"));
        outtextxy(BOARD_X, BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 95, _T("�ո� : ֱ������"));
        outtextxy(BOARD_X, BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 120, _T("R : ���¿�ʼ"));
        outtextxy(BOARD_X, BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE + 145, _T("ESC : �˳�"));

        // ������Ϸ������ʾ
        if (gameOver) {
            settextcolor(RGB(255, 50, 50));
            settextstyle(36, 0, _T("Arial"));
            outtextxy(BOARD_X + BOARD_WIDTH * BLOCK_SIZE / 2 - 100,
                BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE / 2 - 50,
                _T("��Ϸ����!"));

            settextstyle(24, 0, _T("Arial"));
            outtextxy(BOARD_X + BOARD_WIDTH * BLOCK_SIZE / 2 - 80,
                BOARD_Y + BOARD_HEIGHT * BLOCK_SIZE / 2 + 10,
                _T("��R�����¿�ʼ"));
        }

        // ���Ʊ���
        settextcolor(RGB(100, 200, 255));
        settextstyle(48, 0, _T("Arial"));
        outtextxy(WINDOW_WIDTH / 2 - 120, 10, _T("����˹����"));
    }

    bool isGameOver() const { return gameOver; }
};

int main() {
    initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);
    setbkcolor(BLACK);
    cleardevice();

    // ����˫����
    BeginBatchDraw();

    Tetris game;
    double lastTime = GetTickCount() / 1000.0;

    while (true) {
        // ����ʱ������
        double currentTime = GetTickCount() / 1000.0;
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // �����������
        if (_kbhit()) {
            int key = _getch();
            switch (key) {
            case 'a':
            case 'A':
            case 75: // ���ͷ
                game.move(-1, 0);
                break;
            case 'd':
            case 'D':
            case 77: // �Ҽ�ͷ
                game.move(1, 0);
                break;
            case 's':
            case 'S':
            case 80: // �¼�ͷ
                game.move(0, 1);
                break;
            case 'w':
            case 'W':
            case 72: // �ϼ�ͷ
                game.rotate();
                break;
            case ' ': // �ո�
                game.drop();
                break;
            case 'r':
            case 'R': // ���¿�ʼ
                game.reset();
                break;
            case 27: // ESC
                return 0;
            }
        }

        // ������Ϸ״̬
        game.update(currentTime);

        // ������Ϸ
        cleardevice();
        game.draw();

        // ˢ����ʾ
        FlushBatchDraw();

        // ����֡��
        Sleep(10);
    }

    EndBatchDraw();
    closegraph();
    return 0;
}