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
    bool lost;
    int frameCount;
    SquareType grid[GRID_HORIZONTAL_SIZE][GRID_VERTICAL_SIZE];
} GameState;

void InitGame(GameState*);
void DrawFrame(GameState*);
void DrawFilledInGrid(GameState*);
void DrawBoard(GameState*);
void DrawTopBar(GameState*);
void CheckIfAnyFilledLine(GameState*);

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

void NewPiece(GameState* gs) {
    gs->currX = GRID_HORIZONTAL_SIZE/2;
    gs->currY = 0;
    if (gs->grid[gs->currX][0] == BLOCK) {
        printf("You lost. Score: %d\n", gs->score);
        gs->lost = true;
        return;
    }
    gs->grid[gs->currX][0] = MOVING;
    gs->frameCount = 0;
    gs->movingPiece = true;
}

void InitGame(GameState* gs) {
    for (int i = 0; i < GRID_HORIZONTAL_SIZE; i++) {
        for (int j = 0; j < GRID_VERTICAL_SIZE; j++) {
            gs->grid[i][j] = EMPTY;
        }
    }

    gs->currX = GRID_HORIZONTAL_SIZE/2;
    gs->currY = 0;
    gs->score = 0;
    gs->lost = false;
    gs->frameCount = 0;
    NewPiece(gs);
}


void MovePiece(GameState* gs, int dx, int dy) {
    int i = gs->currX;
    int j = gs->currY;

    if (j == GRID_VERTICAL_SIZE-1) {
        gs->grid[i][j] = BLOCK;
        gs->movingPiece = false;
        return;
    }
    if (0 <= i+dx && i+dx < GRID_HORIZONTAL_SIZE && 0 <= j+dy && j+dy < GRID_VERTICAL_SIZE) {
        if (gs->grid[i][j+dy] == BLOCK) {
            gs->grid[i][j] = BLOCK;
            gs->movingPiece = false;
            return;
        }
        gs->grid[i][j] = EMPTY;
        gs->currX = gs->currX + dx;
        gs->currY = gs->currY + dy;
        gs->grid[gs->currX][gs->currY] = MOVING;
    }
}

void MovePieceToBottom(GameState* gs) {
    int i = gs->currX;
    int j = gs->currY;
    gs->grid[i][j] = EMPTY;

    while (true) {
        if (gs->grid[i][j+1] == BLOCK) {
            gs->grid[i][j] = BLOCK;
            gs->movingPiece = false;
            return;
        }
        if (j == GRID_VERTICAL_SIZE-1) {
            gs->grid[i][j] = BLOCK;
            gs->movingPiece = false;
            return;
        }
        j++;
    }
}

void DrawFrame(GameState* gs) {
    if (gs->lost) {
        char* str = malloc(50*sizeof(char));
        sprintf(str, "You lost. Score: %d", gs->score);
        DrawText(str, 0, 0, 20, WHITE);
        free(str);
        ClearBackground(BLACK);
        return;
    }
    if (!gs->movingPiece) {
        NewPiece(gs);
    }
    gs->frameCount++;
    if (gs->frameCount%TICK_RATE == 0) {
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

    ClearBackground(BLACK);
    DrawTopBar(gs);
    DrawBoard(gs);
    CheckIfAnyFilledLine(gs);
    DrawFilledInGrid(gs);
}

void CheckIfAnyFilledLine(GameState* gs) {
    bool filledLines[GRID_VERTICAL_SIZE];
    for (int j = 0; j < GRID_VERTICAL_SIZE; j++) {

        bool fullLine = true;
        for (int i = 0; i < GRID_HORIZONTAL_SIZE; ++i) {
            if (gs->grid[i][j] != BLOCK) {
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
            newGrid[x][newGridIdx] = gs->grid[x][i];
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
            gs->grid[i][j] = newGrid[i][j];
        }
    }
}

void DrawFilledInGrid(GameState* gs) {
    for (int i = 0; i < GRID_HORIZONTAL_SIZE; ++i) {
        for (int j = 0; j < GRID_VERTICAL_SIZE; ++j) {
            if (gs->grid[i][j] == BLOCK) {
                DrawRectangle(i*SQUARE_SIZE, TOP_BAR+j*SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, BLUE);
            }
            if (gs->grid[i][j] == MOVING) {
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
