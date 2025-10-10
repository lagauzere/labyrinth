#include <stdio.h>
#include <stdlib.h>
#include "etape2.h"
#include "etape1.h"
#include <stdbool.h>
#include <wchar.h>


// overwrite file if exists, if not create it
void saveLabyrinthInFile(Labyrinth* labyrinth, const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

     // write dimensions
    fprintf(file, "%d %d\n", labyrinth->rows, labyrinth->columns);

      // write player position
    fprintf(file, "%d %d\n", labyrinth->playerPosition[0], labyrinth->playerPosition[1]);

    // Write labyrinth data to file
    for (int i = 0; i < labyrinth->rows; i++) {
        for (int j = 0; j < labyrinth->columns; j++) {
            fputc(labyrinth->map[i][j], file);
        }
        fputc('\n', file);
    }
    fclose(file);
}


void loadLabyrinthFromFile(Labyrinth* labyrinth, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }

    // Read dimensions
    int rows, columns;
    fscanf(file, "%d %d\n", &rows, &columns);
    *labyrinth = initLabyrinth(rows, columns);

    printf("rows: %d, columns: %d\n", rows, columns);

    // read player position
    fscanf(file, "%d %d\n", &(*labyrinth).playerPosition[0], &(*labyrinth).playerPosition[1]);
    

    // Read labyrinth data from file
    for (int i = 0; i < labyrinth->rows; i++) {
        for (int j = 0; j < labyrinth->columns; j++) {
            int c = fgetc(file);
            if (c != EOF) {
                labyrinth->map[i][j] = (char)c;
            } else {
                labyrinth->map[i][j] = ' '; 
            }
        }
        fgetc(file); 
    }
    fclose(file);
}

void newLabyrinth(){
    printf("merci de donner les dimensions du labyrinth (largeur hauteur):\n");
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
    Labyrinth labyrinth = initLabyrinth(height, width);
    createLabyrinth(&labyrinth);
    displayLabyrinth(&labyrinth);
    saveLabyrinthInFile(&labyrinth, "save.txt");
    freeLabyrinth(&labyrinth);
    printf("labyrinth libéré\n");
}

void startGame(){
    // Implementation of startGame function
}

void loadGame(){
    Labyrinth labyrinth;
    loadLabyrinthFromFile(&labyrinth, "save.txt");
    displayLabyrinth(&labyrinth);
    freeLabyrinth(&labyrinth);
    // Implementation of loadGame function  
}

void exitGame(){
    // Implementation of exitGame function
}

void openMenu(){
    printf("Merci de choisir une option:\n");
    printf("1. Nouveau Labyrinth\n");
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


