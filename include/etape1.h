#ifndef ETAPE1_H
#define ETAPE1_H
#include <wchar.h>

#define UTF8_WALL "█"
#define WALL '#'
#define PATH ' '
#define PLAYER 'o'
#define EXIT '-'
#define ENTRANCE '~'

typedef struct Labyrinth {
    int rows;
    int columns;
    char **map;
    int playerPosition[2];
} Labyrinth;


Labyrinth initLabyrinth(int rows, int columns);

void freeLabyrinth(Labyrinth* labyrinth);

void displayLabyrinth(const Labyrinth* labyrinth);

void createLabyrinth(Labyrinth* labyrinth);

#endif