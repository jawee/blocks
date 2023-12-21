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
#define MOVEMENT_SPEED SQUARE_SIZE

#define TICK_RATE 40

int piecePosX = SCREEN_WIDTH/2;
int piecePosY = 0;

typedef enum GridSquare { EMPTY, MOVING, FULL, BLOCK, FADING } GridSquare;

static GridSquare grid [GRID_HORIZONTAL_SIZE][GRID_VERTICAL_SIZE];
static GridSquare piece [4][4];
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

void InitGame(void) {
    for (int i = 0; i < GRID_HORIZONTAL_SIZE; i++) {
        for (int j = 0; j < GRID_VERTICAL_SIZE; j++) {
                grid[i][j] = EMPTY;
        }
    }

    // Initialize incoming piece matrices
    // for (int i = 0; i < 4; i++) {
    //     for (int j = 0; j< 4; j++) {
    //         incomingPiece[i][j] = EMPTY;
    //     }
    // }
}

int frameCount = 0;
void DrawFrame(void) {
    frameCount++;
    if (frameCount%TICK_RATE == 0) {
        if (piecePosY != SCREEN_HEIGHT-SQUARE_SIZE) {
            piecePosY = piecePosY + MOVEMENT_SPEED;
        }
    }

    if (IsKeyPressed(KEY_LEFT)) {
        if (piecePosX != 0) {
            piecePosX = piecePosX-MOVEMENT_SPEED;
        }
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        if (piecePosX != SCREEN_WIDTH-SQUARE_SIZE) {
            piecePosX = piecePosX+MOVEMENT_SPEED;
        }
    }
    if (IsKeyPressed(KEY_DOWN)) {
        piecePosY = SCREEN_HEIGHT-SQUARE_SIZE;
    }

    DrawBoard();
    ClearBackground(BLACK);
    DrawRectangle(piecePosX, piecePosY, SQUARE_SIZE, SQUARE_SIZE, RED);
    DrawFilledInGrid();
}

void DrawFilledInGrid(void) {
    for (int i = 0; i < GRID_HORIZONTAL_SIZE; ++i) {
        for (int j = 0; j < GRID_VERTICAL_SIZE; ++j) {
            if (grid[i][j] == BLOCK) {
                DrawRectangle(i*SQUARE_SIZE, j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, BLUE);
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






