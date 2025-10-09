#ifndef ETAPE1_H
#define ETAPE1_H
#include <wchar.h>

typedef struct Labyrinthe {
    int rows;
    int columns;
    wchar_t **map;
    int playerPosition[2];
} Labyrinthe;


Labyrinthe initLabyrinthe(int rows, int columns);

void freeLabyrinthe(Labyrinthe* labyrinthe);

void displayLabyrinthe(const Labyrinthe* labyrinthe);

void createLabyrinthe(Labyrinthe* labyrinthe);

#endif