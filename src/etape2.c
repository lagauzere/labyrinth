#include <stdio.h>
#include <stdlib.h>
#include "etape2.h"
#include "etape1.h"
#include <stdbool.h>
#include <wchar.h>



void newLabyrinth(){
    printf("merci de donner les dimensions du labyrinthe (largeur hauteur):\n");
    printf("les doivent être impairs et supérieurs à 5\n");
    int width = 0, height = 0;
    while (width % 2 == 0 || height % 2 == 0 || width < 5 || height < 5)
    {
        scanf("%d %d", &width, &height);
        if (width % 2 == 0 || height % 2 == 0 || width < 5 || height < 5)
        {
            printf("dimensions invalides, merci de réessayer:\n");
        }
    }
    Labyrinthe labyrinthe = initLabyrinthe(height, width);
    createLabyrinthe(&labyrinthe);
    displayLabyrinthe(&labyrinthe);
    freeLabyrinthe(&labyrinthe);
    printf("labyrinthe libéré\n");
}

void startGame(){
    // Implementation of startGame function
}

void loadGame(){
    // Implementation of loadGame function  
}

void exitGame(){
    // Implementation of exitGame function
}

void openMenu(){
    printf("Merci de choisir une option:\n");
    printf("1. Nouveau Labyrinthe\n");
    printf("commencer une partie\n");
    printf("2. Charger une partie\n");
    printf("3. Quitter\n");
    int choice;
    scanf("%d", &choice);
    switch(choice){
        case 1:
            newLabyrinth();
            startGame();
            break;
        case 2:
            loadGame();
            break;
        case 3:
            exitGame();
            break;
    }
}


