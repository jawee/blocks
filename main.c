#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define SCREEN_WIDTH 450
#define SCREEN_HEIGHT TOP_BAR+BOARD_HEIGHT 

#define TOP_BAR 25

#define BOARD_WIDTH 450
#define BOARD_HEIGHT 720
#define GRID_HORIZONTAL_SIZE 10
#define GRID_VERTICAL_SIZE 16

#define SQUARE_SIZE BOARD_WIDTH/GRID_HORIZONTAL_SIZE

#define TICK_RATE 30

void InitGame(void);
void DrawFrame(void);
void DrawFilledInGrid(void);
void DrawBoard(void);
void DrawTopBar(void);
void CheckIfAnyFilledLine(void);

static int score = 0;

typedef enum SquareType { EMPTY, MOVING, BLOCK } SquareType;

SquareType grid [GRID_HORIZONTAL_SIZE][GRID_VERTICAL_SIZE];

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "blocks");
    SetTargetFPS(60);

    InitGame();

    while (!WindowShouldClose()) {
        BeginDrawing();

        DrawFrame();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

static int currX = GRID_HORIZONTAL_SIZE/2;
static int currY = 0;
static int frameCount = 0;
static bool movingPiece = false;

void NewPiece(void) {
    currX = GRID_HORIZONTAL_SIZE/2;
    currY = 0;
    grid[currX][0] = MOVING;
    frameCount = 0;
    movingPiece = true;
}

void InitGame(void) {
    for (int i = 0; i < GRID_HORIZONTAL_SIZE; i++) {
        for (int j = 0; j < GRID_VERTICAL_SIZE; j++) {
            grid[i][j] = EMPTY;
        }
    }

    NewPiece();
}


void MovePiece(int dx, int dy) {
    int i = currX;
    int j = currY;

    if (j == GRID_VERTICAL_SIZE-1) {
        grid[i][j] = BLOCK;
        movingPiece = false;
        return;
    }
    if (0 <= i+dx && i+dx < GRID_HORIZONTAL_SIZE && 0 <= j+dy && j+dy < GRID_VERTICAL_SIZE) {
        if (grid[i][j+dy] == BLOCK) {
            grid[i][j] = BLOCK;
            movingPiece = false;
            return;
        }
        grid[i][j] = EMPTY;
        currX = currX + dx;
        currY = currY + dy;
        grid[currX][currY] = MOVING;
    }
}

void MovePieceToBottom(void) {
    int i = currX;
    int j = currY;
    grid[i][j] = EMPTY;

    while (true) {
        if (grid[i][j+1] == BLOCK) {
            grid[i][j] = BLOCK;
            movingPiece = false;
            return;
        }
        if (j == GRID_VERTICAL_SIZE-1) {
            grid[i][j] = BLOCK;
            movingPiece = false;
            return;
        }
        j++;
    }
}

void DrawFrame(void) {
    if (!movingPiece) {
        NewPiece();
    }
    frameCount++;
    if (frameCount%TICK_RATE == 0) {
        MovePiece(0, 1);
    }

    if (IsKeyPressed(KEY_LEFT)) {
        MovePiece(-1, 0);
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        MovePiece(1, 0);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        MovePieceToBottom();
    }

    DrawTopBar();
    DrawBoard();
    ClearBackground(BLACK);
    CheckIfAnyFilledLine();
    DrawFilledInGrid();
}

void CheckIfAnyFilledLine(void) {
    bool filledLines[GRID_VERTICAL_SIZE];
    for (int j = 0; j < GRID_VERTICAL_SIZE; j++) {

        bool fullLine = true;
        for (int i = 0; i < GRID_HORIZONTAL_SIZE; ++i) {
            if (grid[i][j] != BLOCK) {
                fullLine = false;
                break;
            }
        }

        filledLines[j] = fullLine;
    }

    SquareType newGrid[GRID_HORIZONTAL_SIZE][GRID_VERTICAL_SIZE];
    
    for (int i = 0; i < GRID_HORIZONTAL_SIZE; i++) {
        for (int j = 0; j < GRID_VERTICAL_SIZE; j++) {
            newGrid[i][j] = EMPTY;
        }
    }
    int newGridIdx = GRID_VERTICAL_SIZE-1;
    // remove filled lines and move everything above down
    for (int i = GRID_VERTICAL_SIZE-1; 0 <= i; --i) {
        if (filledLines[i]) {
            score++;
            continue;
        }
        for (int x = 0; x < GRID_HORIZONTAL_SIZE; ++x) {
            newGrid[x][newGridIdx] = grid[x][i];
        }
        newGridIdx--;
    }

    for (int i = 0; i < newGridIdx; ++i) {
        for (int x = 0; x < GRID_HORIZONTAL_SIZE; ++x) {
            newGrid[x][i] = EMPTY;
        }
    }

    for (int i = 0; i < GRID_HORIZONTAL_SIZE; i++) {
        for (int j = 0; j < GRID_VERTICAL_SIZE; j++) {
            grid[i][j] = newGrid[i][j];
        }
    }
}

void DrawFilledInGrid(void) {
    for (int i = 0; i < GRID_HORIZONTAL_SIZE; ++i) {
        for (int j = 0; j < GRID_VERTICAL_SIZE; ++j) {
            if (grid[i][j] == BLOCK) {
                DrawRectangle(i*SQUARE_SIZE, TOP_BAR+j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, BLUE);
            }
            if (grid[i][j] == MOVING) {
                DrawRectangle(i*SQUARE_SIZE, TOP_BAR+j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, RED);
            }
        }
    }
}

void DrawBoard(void) {
    for (int i = 0; i < SCREEN_WIDTH; i += SQUARE_SIZE) {
        DrawLine(i, TOP_BAR, i, SCREEN_HEIGHT, WHITE);
    }
    for (int j = TOP_BAR; j < SCREEN_HEIGHT; j += SQUARE_SIZE) {
        DrawLine(0, j, SCREEN_WIDTH, j, WHITE);
    }
}

void DrawTopBar(void) {
    char* str = malloc(sizeof(char) * 50);
    sprintf(str, "score %i", score);
    DrawText(str, 0, 0, 20, WHITE);
    free(str);
}
