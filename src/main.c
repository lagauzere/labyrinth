#include <stdio.h>
#include <stdlib.h>
#include "etape1.h"
#include "etape2.h"
#include <locale.h>
#include <time.h>

int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "");
    Labyrinthe labyrinthe = initLabyrinthe(11, 25);
    createLabyrinthe(&labyrinthe);
    displayLabyrinthe(&labyrinthe);
    freeLabyrinthe(&labyrinthe);
    return 0;
}