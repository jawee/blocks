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

typedef enum { EMPTY, MOVING, BLOCK } SquareType;
typedef struct {
    bool movingPiece;
    int score;
    int currX;
    int currY;
} GameState;

void InitGame(GameState*);
void DrawFrame(GameState*);
void DrawFilledInGrid(GameState*);
void DrawBoard(GameState*);
void DrawTopBar(GameState*);
void CheckIfAnyFilledLine(GameState*);

SquareType grid [GRID_HORIZONTAL_SIZE][GRID_VERTICAL_SIZE];

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "blocks");
    SetTargetFPS(60);

    GameState gs = {0};
    InitGame(&gs);

    while (!WindowShouldClose()) {
        BeginDrawing();

        DrawFrame(&gs);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}

static int frameCount = 0;
static bool movingPiece = false;

void NewPiece(GameState* gs) {
    gs->currX = GRID_HORIZONTAL_SIZE/2;
    gs->currY = 0;
    grid[gs->currX][0] = MOVING;
    frameCount = 0;
    // movingPiece = true;
    gs->movingPiece = true;
}

void InitGame(GameState* gs) {
    for (int i = 0; i < GRID_HORIZONTAL_SIZE; i++) {
        for (int j = 0; j < GRID_VERTICAL_SIZE; j++) {
            grid[i][j] = EMPTY;
        }
    }

    gs->currX = GRID_HORIZONTAL_SIZE/2;
    gs->currY = 0;
    NewPiece(gs);
}


void MovePiece(GameState* gs, int dx, int dy) {
    int i = gs->currX;
    int j = gs->currY;

    if (j == GRID_VERTICAL_SIZE-1) {
        grid[i][j] = BLOCK;
        gs->movingPiece = false;
        return;
    }
    if (0 <= i+dx && i+dx < GRID_HORIZONTAL_SIZE && 0 <= j+dy && j+dy < GRID_VERTICAL_SIZE) {
        if (grid[i][j+dy] == BLOCK) {
            grid[i][j] = BLOCK;
            gs->movingPiece = false;
            return;
        }
        grid[i][j] = EMPTY;
        gs->currX = gs->currX + dx;
        gs->currY = gs->currY + dy;
        grid[gs->currX][gs->currY] = MOVING;
    }
}

void MovePieceToBottom(GameState* gs) {
    int i = gs->currX;
    int j = gs->currY;
    grid[i][j] = EMPTY;

    while (true) {
        if (grid[i][j+1] == BLOCK) {
            grid[i][j] = BLOCK;
            gs->movingPiece = false;
            return;
        }
        if (j == GRID_VERTICAL_SIZE-1) {
            grid[i][j] = BLOCK;
            gs->movingPiece = false;
            return;
        }
        j++;
    }
}

void DrawFrame(GameState* gs) {
    if (!gs->movingPiece) {
        NewPiece(gs);
    }
    frameCount++;
    if (frameCount%TICK_RATE == 0) {
        MovePiece(gs, 0, 1);
    }

    if (IsKeyPressed(KEY_LEFT)) {
        MovePiece(gs, -1, 0);
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        MovePiece(gs, 1, 0);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        MovePieceToBottom(gs);
    }

    DrawTopBar(gs);
    DrawBoard(gs);
    ClearBackground(BLACK);
    CheckIfAnyFilledLine(gs);
    DrawFilledInGrid(gs);
}

void CheckIfAnyFilledLine(GameState* gs) {
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
            gs->score++;
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

void DrawFilledInGrid(GameState* gs) {
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

void DrawBoard(GameState* gs) {
    for (int i = 0; i < SCREEN_WIDTH; i += SQUARE_SIZE) {
        DrawLine(i, TOP_BAR, i, SCREEN_HEIGHT, WHITE);
    }
    for (int j = TOP_BAR; j < SCREEN_HEIGHT; j += SQUARE_SIZE) {
        DrawLine(0, j, SCREEN_WIDTH, j, WHITE);
    }
}

void DrawTopBar(GameState* gs) {
    char* str = malloc(sizeof(char) * 50);
    sprintf(str, "score %i", gs->score);
    DrawText(str, 0, 0, 20, WHITE);
    free(str);
}
