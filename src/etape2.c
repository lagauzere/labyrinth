#include <stdio.h>
#include <stdlib.h>
#include "etape2.h"
#include "etape1.h"
#include <stdbool.h>
#include <wchar.h>
#include <stdio_ext.h>
#include <string.h>

// top 10 leaderboard management

bool isScoreInTop10(int score){
    FILE* file = fopen("leaderboard.score", "r");
    if (file == NULL) {
        return true; // file doesn't exist yet, so any score is in top 10
    }
    int scores[10];
    int count = 0;
    while (fscanf(file, "%*s %d", &scores[count]) != EOF && count < 10) {
        count++;
    }
    fclose(file);
    if (count < 10) {
        return true; // less than 10 scores, so any score is in top 10
    }
    for (int i = 0; i < 10; i++) {
        if (score > scores[i]) {
            return true;
        }
    }
    return false;
}

void updateTop10(int score, const char* playerName){
    FILE* file = fopen("leaderboard.score", "r");
    int scores[10];
    char names[10][50];
    int count = 0;
    if (file != NULL) {
        while (fscanf(file, "%49s %d", names[count], &scores[count]) != EOF && count < 10) {
            count++;
        }
        fclose(file);
    }
    // add new score
    scores[count] = score;
    strcpy(names[count], playerName);
    count++;

    // sort scores
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (scores[j] > scores[i]) {
                int tempScore = scores[i];
                scores[i] = scores[j];
                scores[j] = tempScore;
                char tempName[50];
                strcpy(tempName, names[i]);
                strcpy(names[i], names[j]);
                strcpy(names[j], tempName);
            }
        }
    }

    // write back top 10
    file = fopen("leaderboard.score", "w");
    for (int i = 0; i < count && i < 10; i++) {
        fprintf(file, "%s %d\n", names[i], scores[i]);
    }
    fclose(file);
}
   

void displayTop10(){
     __fpurge(stdin);
    system("clear"); 
    FILE* file = fopen("leaderboard.score", "r");
    if (file == NULL) {
        printf("Aucun fichier de score trouvé.\n");
        printf("Appuyez sur Entrée pour continuer...");
        getchar();
        return;
    }
    printf("----- Top 10 Scores -----\n");
    char name[50];
    int score;
    int rank = 1;
    while (fscanf(file, "%49s %d", name, &score) != EOF) {
        printf("%d. %s - %d\n", rank, name, score);
        rank++;
    }
    fclose(file);
    printf("-------------------------\n");
    printf("Appuyez sur Entrée pour continuer...");
    getchar();
   
}

int removeCoordinateInCoordinatesArray(int * coords, int ** coordinatesArray, int size){
    for(int i = 0; i< size; i++){
        if(coords[0] == coordinatesArray[i][0] && coords[1] == coordinatesArray[i][1]){
            coordinatesArray[i][0] = -1;
            coordinatesArray[i][1] = -1;
            return 0;
        }
    }
    return -1;
}

bool isExitReached(Labyrinth* labyrinth){
    if(labyrinth->map[labyrinth->playerPosition[0]][labyrinth->playerPosition[1]] == EXIT){
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
        if(labyrinth->map[newRow][newCol] == WALL || (labyrinth->map[newRow][newCol] == EXIT && !labyrinth->hasKey)){
          return; // blocked by wall or closed door
        }
        if(newRow == labyrinth->keyPosition[0] && newCol == labyrinth->keyPosition[1] && labyrinth->hasKey == 0){
            labyrinth->hasKey = 1;

            // remove key from map
            labyrinth->keyPosition[0] = -1; 
            labyrinth->keyPosition[1] = -1;
        }
        if(isCoordinateInCoordinatesArray((int[]){newRow, newCol}, labyrinth->treasuresPositions, TREASURES)){
            labyrinth->score += TREASURE_VALUE;
            removeCoordinateInCoordinatesArray((int[]){newRow, newCol}, labyrinth->treasuresPositions, TREASURES);
        }
        if(isCoordinateInCoordinatesArray((int[]){newRow, newCol}, labyrinth->trapPositions, TRAPS)){
            labyrinth->score += TRAP_VALUE;
            removeCoordinateInCoordinatesArray((int[]){newRow, newCol}, labyrinth->trapPositions, TREASURES);
        }
    
        labyrinth->playerPosition[0] = newRow;
        labyrinth->playerPosition[1] = newCol;
        labyrinth->score += MOVE_PENALTY;
    }
}

// overwrite file if exists, if not create it ( write and read order matters)
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

    // write hasKey
    fprintf(file, "%d\n", labyrinth->hasKey);

    // write key position
    fprintf(file, "%d %d\n", labyrinth->keyPosition[0], labyrinth->keyPosition[1]);

    // write treasures positions
    for(int i = 0; i < TREASURES; i++){
        fprintf(file, "%d %d\n", labyrinth->treasuresPositions[i][0], labyrinth->treasuresPositions[i][1]);
    }

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

    // read player position
    fscanf(file, "%d %d\n", &(*labyrinth).playerPosition[0], &(*labyrinth).playerPosition[1]);
    
    // read hasKey
    fscanf(file, "%d\n", &(*labyrinth).hasKey);

    // read key position
    fscanf(file, "%d %d\n", &(*labyrinth).keyPosition[0], &(*labyrinth).keyPosition[1]);

    // read treasures positions
    for(int i = 0; i < TREASURES; i++){
        fscanf(file, "%d %d\n", &(*labyrinth).treasuresPositions[i][0], &(*labyrinth).treasuresPositions[i][1]);
    }

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
    system("clear"); 
    printf("merci de donner les dimensions du labyrinth (largeur hauteur):\n");
    printf("les dimensions doivent être impairs et supérieurs à 5\n");
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
    system("clear"); 
    displayLabyrinth(labyrinth);
    printf("Merci de donner le nom du fichier de sauvegarde :\n");
    char *filename = malloc(100 * sizeof(char));
    scanf("%s", filename);
    strcat(filename, EXTENSION);
    saveLabyrinthInFile(labyrinth, filename);
    free(filename);
}


void startGame(Labyrinth* labyrinth ){
    // check if labyrinth is loaded
    if(labyrinth->map == NULL){
        __fpurge(stdin);
        system("clear");
        printf("Aucun labyrinth chargé. Merci de créer ou charger un labyrinth avant de commencer une partie.\n");
        printf("Appuyez sur Entrée pour continuer...");
        getchar();
        return;
    }
    __fpurge(stdin);
    system("clear"); 
    char move = ' ';
    printf("Entrez votre mouvement (z: haut, s: bas, q: gauche, d: droite):\n");
    printf("Score: %d\n\n", labyrinth->score);
    displayLabyrinth(labyrinth);
    while (!isExitReached(labyrinth)) {
        __fpurge(stdin);
        move = getchar();
        handlePlayerMovement(labyrinth, move);
        system("clear"); 
        printf("Entrez votre mouvement (z: haut, s: bas, q: gauche, d: droite):\n");
        printf("Score: %d\n\n", labyrinth->score);
        displayLabyrinth(labyrinth);
    }
    __fpurge(stdin);
    system("clear"); 
    printf("\nFélicitations! Vous avez terminé le labyrinth!\n");
    printf("Votre score final est de %d points!\n", labyrinth->score);
    printf("Appuyez sur Entrée pour continuer...");
    getchar();
    if (isScoreInTop10(labyrinth->score)) {
        printf("Vous êtes dans le top 10! Entrez votre nom:\n");
        char playerName[50];
        __fpurge(stdin);
        fgets(playerName, sizeof(playerName), stdin);
        playerName[strcspn(playerName, "\n")] = 0; 
        updateTop10(labyrinth->score, playerName);
        displayTop10();
    } 
}


void listSaveFiles(char*** files, int* fileCount) {
    FILE* pipe = popen("ls *.cfg 2>/dev/null", "r");
    if (pipe == NULL) {
        perror("Erreur lors de l'ouverture du pipe");
        return;
    }

    char buffer[256];
    *fileCount = 0;
    *files = NULL;

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; 
        *files = realloc(*files, (*fileCount + 1) * sizeof(char*));
        (*files)[*fileCount] = strdup(buffer);
        (*fileCount)++;
    }
    pclose(pipe);
    
}


void loadGame(Labyrinth* labyrinth){
    __fpurge(stdin);
    system("clear"); 
    char **files = NULL;
    int fileCount = 0;
    listSaveFiles(&files, &fileCount);
    if (fileCount == 0) {
        printf("Aucun fichier de sauvegarde trouvé. Appuyez sur Entrée pour continuer...");
        getchar();
    }else{
        for (int i = 0; i < fileCount; i++) {
            printf("%d. %s\n", i + 1, files[i]);
        }
        printf("Entrez le numéro du fichier à charger:\n");
        int choice = 0;
        while (choice < 1 || choice > fileCount) {
            scanf("%d", &choice);
            if (choice < 1 || choice > fileCount) {
                printf("Choix invalide, merci de réessayer:\n");
            }
        }
        char* filename = files[choice - 1];
        loadLabyrinthFromFile(labyrinth, filename);
    }
}

void openMenu(){
    Labyrinth labyrinth;
    labyrinth.map = NULL;
    while (1)
    {
        __fpurge(stdin);
        system("clear"); 
        if(labyrinth.map != NULL){
            printf("Labyrinth chargé : \n\n");
            displayLabyrinth(&labyrinth);
        }
        else{
            printf("Aucun labyrinth chargé.\n");
        }
        printf("\nMerci de choisir une option:\n");
        printf("1. Nouveau Labyrinth\n");
        printf("2. Commencer une partie\n");
        printf("3. Charger une partie\n");
        printf("4. Afficher le top 10\n");
        printf("5. Quitter\n");
        int choice;
        scanf("%d", &choice);
        switch(choice){
            case 1:
                newLabyrinth(&labyrinth);
                break;
            case 2:
                startGame(&labyrinth);
                // free labyrinth after game
                freeLabyrinth(&labyrinth);
                labyrinth.map = NULL;
                break;
            case 3:
            loadGame(&labyrinth);
                break;
            case 4:
                displayTop10();
                break;
            case 5:
                printf("Au revoir!\n");
                if(labyrinth.map != NULL){
                    freeLabyrinth(&labyrinth);
                }
                return;
            break;
        }
    }
}


