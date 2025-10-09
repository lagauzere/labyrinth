#include <stdio.h>
#include <stdlib.h>
#include "etape1.h"

Labyrinthe initLabyrinthe(int rows, int columns){
    Labyrinthe labyrinthe;
    labyrinthe.map = malloc(rows * sizeof(wchar_t*));
    for(int i = 0; i < rows; i++){
        labyrinthe.map[i] = malloc(columns * sizeof(wchar_t));
    }
    labyrinthe.rows = rows;
    labyrinthe.columns = columns;
    labyrinthe.playerPosition[0] = 0;
    labyrinthe.playerPosition[1] = 1;
    return labyrinthe;
}

void freeLabyrinthe(Labyrinthe* labyrinthe){
    for(int i = 0; i < labyrinthe->rows; i++){
        free(labyrinthe->map[i]);
    }
    free(labyrinthe->map);
}

void displayLabyrinthe(const Labyrinthe* labyrinthe){
    // Implementation of displayLabyrinthe function
}

void createLabyrinthe(Labyrinthe* labyrinthe){
    // Implementation of createLabyrinthe function

}