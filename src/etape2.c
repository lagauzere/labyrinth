#include <stdio.h>
#include <stdlib.h>
#include "etape2.h"
#include "etape1.h"
#include <stdbool.h>
#include <wchar.h>
#include <stdio_ext.h>
#include <string.h>


bool isExitReached(Labyrinth* labyrinth){
    if(labyrinth->map[labyrinth->playerPosition[0]][labyrinth->playerPosition[1]] == EXIT){
        printf("Félicitations! Vous avez atteint la sortie!\n");
        return true;
    }
    return false;
}


void handlePlayerMovement(Labyrinth* labyrinth, char direction){
    int newRow = labyrinth->playerPosition[0];
    int newCol = labyrinth->playerPosition[1];

    switch(direction){
        case 'z': // Up
            newRow--;
            break;
        case 's': // Down
            newRow++;
            break;
        case 'q': // Left
            newCol--;
            break;
        case 'd': // Right
            newCol++;
            break;
        default:
            return; // Invalid input
    }

    if(newRow >= 0 && newRow < labyrinth->rows && newCol >= 0 && newCol < labyrinth->columns){
        if(labyrinth->map[newRow][newCol] != WALL){
            labyrinth->playerPosition[0] = newRow;
            labyrinth->playerPosition[1] = newCol;
        }
    }
}

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

void newLabyrinth(Labyrinth* labyrinth){
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
    *labyrinth = initLabyrinth(height, width);
    createLabyrinth(labyrinth);
    displayLabyrinth(labyrinth);
    printf("Merci de donner le nom du fichier de sauvegarde :\n");
    char *filename = malloc(100 * sizeof(char));
    scanf("%s", filename);
    strcat(filename, EXTENSION);
    saveLabyrinthInFile(labyrinth, filename);
    free(filename);
    printf("labyrinth libéré\n");
}


// ncurses can be used to handle key presses more easily
void startGame(Labyrinth* labyrinth ){
    char move = ' ';
    printf("Entrez votre mouvement (z: haut, s: bas, q: gauche, d: droite):\n");
    displayLabyrinth(labyrinth);
    while (!isExitReached(labyrinth)) {
        // need to find a way to handle single key press without enter
        __fpurge(stdin);
        move = getchar();
        handlePlayerMovement(labyrinth, move);
        system("clear"); 
        displayLabyrinth(labyrinth);
    }
}

void loadGame(Labyrinth* labyrinth){
    char filename[100] = "save.config";
    printf("Entrez le nom du fichier de sauvegarde :\n");
    scanf("%s", filename);

    loadLabyrinthFromFile(labyrinth, filename);
    displayLabyrinth(labyrinth);
    freeLabyrinth(labyrinth);
    // Implementation of loadGame function  
}

void openMenu(){
    Labyrinth labyrinth;
    while (1)
    {
        printf("Merci de choisir une option:\n");
        printf("1. Nouveau Labyrinth\n");
        printf("2. Commencer une partie\n");
        printf("3. Charger une partie\n");
        printf("4. Quitter\n");
        int choice;
        scanf("%d", &choice);
        switch(choice){
            case 1:
                newLabyrinth(&labyrinth);
                break;
            case 2:
                startGame(&labyrinth);
                break;
            case 3:
                loadGame(&labyrinth);
                break;
            case 4:
                printf("Au revoir!\n");
                return;
            break;
        }
    }
}


