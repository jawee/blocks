#include "raylib.h"
#include <stdio.h>

static void InitGame(void);
static void DrawFrame(void);
static void DrawFilledInGrid(void);
static void DrawBoard(void);

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 800

#define GRID_HORIZONTAL_SIZE 12
#define GRID_VERTICAL_SIZE 20

#define SQUARE_SIZE SCREEN_WIDTH/GRID_HORIZONTAL_SIZE

#define TICK_RATE 30

int piecePosX = SCREEN_WIDTH/2;
int piecePosY = 0;

typedef enum GridSquare { EMPTY, MOVING, FULL, BLOCK, FADING } GridSquare;

static GridSquare grid [GRID_HORIZONTAL_SIZE][GRID_VERTICAL_SIZE];
// static GridSquare piece [4][4];
// static GridSquare incomingPiece [4][4];

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
void NewPiece(void) {
    currX = GRID_HORIZONTAL_SIZE/2;
    currY = 0;
    grid[currX][0] = MOVING;
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

int frameCount = 0;
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

    DrawBoard();
    ClearBackground(BLACK);
    // DrawRectangle(piecePosX, piecePosY, SQUARE_SIZE, SQUARE_SIZE, RED);
    DrawFilledInGrid();
}

void DrawFilledInGrid(void) {
    for (int i = 0; i < GRID_HORIZONTAL_SIZE; ++i) {
        for (int j = 0; j < GRID_VERTICAL_SIZE; ++j) {
            if (grid[i][j] == BLOCK) {
                DrawRectangle(i*SQUARE_SIZE, j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, BLUE);
            }

            if (grid[i][j] == MOVING) {
                DrawRectangle(i*SQUARE_SIZE, j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, RED);
            }
        }
    }
}

void DrawBoard(void) {
    for (int i = 0; i < SCREEN_WIDTH; i += SQUARE_SIZE) {
        DrawLine(i, 0, i, SCREEN_HEIGHT, WHITE);
    }
    for (int j = 0; j < SCREEN_HEIGHT; j += SQUARE_SIZE) {
        DrawLine(0, j, SCREEN_WIDTH, j, WHITE);
    }
}






