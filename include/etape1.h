#ifndef ETAPE1_H
#define ETAPE1_H
#include <wchar.h>

#define UTF8_WALL "█"
#define WALL '#'
#define PATH ' '
#define PLAYER 'o'
#define EXIT '-'

typedef struct Labyrinthe {
    int rows;
    int columns;
    char **map;
    int playerPosition[2];
} Labyrinthe;


Labyrinthe initLabyrinthe(int rows, int columns);

void freeLabyrinthe(Labyrinthe* labyrinthe);

void displayLabyrinthe(const Labyrinthe* labyrinthe);

void createLabyrinthe(Labyrinthe* labyrinthe);

#endif