#ifndef ETAPE1_H
#define ETAPE1_H
#include <stdbool.h>

#define UTF8_WALL "█"
#define WALL '#'
#define PATH ' '
#define PLAYER 'o'
#define EXIT '-'
#define ENTRANCE '~'
#define KEY '*'
#define UTF8_DOOR "▀"
#define TREASURE '$'

#define TREASURE_VALUE 10
#define TREASURES 5 

#define TRAP_VALUE -5
#define TRAPS 3

#define MOVE_PENALTY -1

typedef struct Labyrinth {
    int score;
    int hasKey; // 1 if player has the key, 0 otherwise
    int rows;
    int columns;
    char **map;
    int playerPosition[2];
    int keyPosition[2];
    int **treasuresPositions;
    int **trapPositions;
} Labyrinth;


Labyrinth initLabyrinth(int rows, int columns);

void freeLabyrinth(Labyrinth* labyrinth);

void displayLabyrinth(const Labyrinth* labyrinth);

void createLabyrinth(Labyrinth* labyrinth);
bool isCoordinateInCoordinatesArray(int* coords, int **coordinatesArray, int size);
#endif