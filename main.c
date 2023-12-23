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

static int score = 0;

typedef enum SquareType { EMPTY, MOVING, BLOCK } SquareType;

static SquareType grid [GRID_HORIZONTAL_SIZE][GRID_VERTICAL_SIZE];

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

int currX = GRID_HORIZONTAL_SIZE/2;
int currY = 0;
int frameCount = 0;

void NewPiece(void) {
    currX = GRID_HORIZONTAL_SIZE/2;
    currY = 0;
    grid[currX][0] = MOVING;
    frameCount = 0;
    ++score;
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
        NewPiece();
    }
    if (0 <= i+dx && i+dx < GRID_HORIZONTAL_SIZE && 0 <= j+dy && j+dy < GRID_VERTICAL_SIZE) {
        if (grid[i][j+dy] == BLOCK) {
            grid[i][j] = BLOCK;
            NewPiece();
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
            NewPiece();
            return;
        }
        if (j == GRID_VERTICAL_SIZE-1) {
            grid[i][j] = BLOCK;
            NewPiece();
            return;
        }
        j++;
    }
}

void DrawFrame(void) {
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
    DrawFilledInGrid();
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
