#include <stdio.h>
#include <stdlib.h>
#include "etape1.h"
#include "etape2.h"
#include <locale.h>
#include <time.h>

int main() {
    srand(time(NULL));
    // test etape 1
    printf("--- Etape 1 ---\n");
    Labyrinth labyrinth = initLabyrinth(11, 25);
    createLabyrinth(&labyrinth);
    displayLabyrinth(&labyrinth);
    freeLabyrinth(&labyrinth);
    
    // test etape 2
    printf("\n--- Etape 2 ---\n");
    openMenu();
    return 0;
}