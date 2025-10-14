#ifndef ETAPE1_H
#define ETAPE1_H
#include <wchar.h>

#define UTF8_WALL "█"
#define WALL '#'
#define PATH ' '
#define PLAYER 'o'
#define EXIT '-'
#define ENTRANCE '~'
#define KEY '*'
#define DOOR '+'

typedef struct Labyrinth {
    int hasKey; // 1 if player has the key, 0 otherwise
    int rows;
    int columns;
    char **map;
    int playerPosition[2];
    int keyPosition[2];
} Labyrinth;


Labyrinth initLabyrinth(int rows, int columns);

void freeLabyrinth(Labyrinth* labyrinth);

void displayLabyrinth(const Labyrinth* labyrinth);

void createLabyrinth(Labyrinth* labyrinth);

#endif