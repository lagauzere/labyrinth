#include <stdio.h>
#include <stdlib.h>
#include "etape1.h"
#include <stdbool.h>
#include <locale.h>

bool isSameCoords(int* pos1, int* pos2){
    return pos1[0] == pos2[0] && pos1[1] == pos2[1];
}

bool isCoordinateInCoordinatesArray(int* coords, int **coordinatesArray, int size){
    for(int i = 0; i < size; i++){
        if(isSameCoords(coords, coordinatesArray[i])){
            return true;
        }
    }
    return false;
}

Labyrinth initLabyrinth(int rows, int columns){
    Labyrinth labyrinth;
    labyrinth.map = malloc(rows * sizeof(wchar_t*));
    for(int i = 0; i < rows; i++){
        labyrinth.map[i] = malloc(columns * sizeof(wchar_t));
    }
    labyrinth.rows = rows;
    labyrinth.columns = columns;
    labyrinth.playerPosition[0] = 0;
    labyrinth.playerPosition[1] = 1;
    labyrinth.hasKey = 0;
    labyrinth.keyPosition[0] = -1; // we dont know dimensions yet
    labyrinth.keyPosition[1] = -1;
    labyrinth.treasuresPositions = malloc(TREASURES * sizeof(int*));
    for(int i = 0; i < TREASURES; i++){
        labyrinth.treasuresPositions[i] = malloc(2 * sizeof(int));
        labyrinth.treasuresPositions[i][0] = -1;
        labyrinth.treasuresPositions[i][1] = -1;
    }

    labyrinth.trapPositions = malloc(TRAPS * sizeof(int*));
    for(int i = 0; i < TRAPS; i++){
        labyrinth.trapPositions[i] = malloc(2 * sizeof(int));
        labyrinth.trapPositions[i][0] = -1;
        labyrinth.trapPositions[i][1] = -1;
    }
    labyrinth.score = 0;
    return labyrinth;
}

void freeLabyrinth(Labyrinth* labyrinth){
    for(int i = 0; i < labyrinth->rows; i++){
        free(labyrinth->map[i]);
    }
    free(labyrinth->map);

    for(int i = 0; i < TREASURES; i++){
        free(labyrinth->treasuresPositions[i]);
    }
    free(labyrinth->treasuresPositions);
}



// god it's messy
void displayLabyrinth(const Labyrinth* labyrinth){
    for(int i = 0; i < labyrinth->rows; i++){
        for(int j = 0; j < labyrinth->columns; j++){
            if (i == labyrinth->playerPosition[0] && j == labyrinth->playerPosition[1]) {
                printf("%c", PLAYER); 
            } else if (i == labyrinth->keyPosition[0] && j == labyrinth->keyPosition[1] && labyrinth->hasKey == 0) {
                printf("%c", KEY); 
            } else if (labyrinth->map[i][j] == WALL) {
                printf("%s", UTF8_WALL); 
            } else if (labyrinth->map[i][j] == EXIT) {
                if(labyrinth->hasKey){
                    printf("%c", EXIT);
                }
                else{
                    printf("%s", UTF8_DOOR);
                }
            } else if (isCoordinateInCoordinatesArray((int[]){i, j}, labyrinth->treasuresPositions, (int)TREASURES)) {
                printf("%c", TREASURE);

            } else if (isCoordinateInCoordinatesArray((int[]){i, j}, labyrinth->trapPositions, (int)TRAPS)) {
                printf("X");
            }
            else {
                printf("%c", labyrinth->map[i][j]);
            } 
        }
        printf("\n");
    }
}

void setRandomTreasuresInLabyrinth(Labyrinth* labyrinth){
    for(int i = 0; i < TREASURES; i++){
        int coords[2];
        do{
            coords[0] = (rand() % (labyrinth->rows / 2)) * 2 + 1;
            coords[1] = (rand() % (labyrinth->columns / 2)) * 2 + 1;
        }while( isSameCoords(coords, labyrinth->keyPosition) 
        || isCoordinateInCoordinatesArray(coords, labyrinth->treasuresPositions, i) 
    || isCoordinateInCoordinatesArray(coords, (int**)labyrinth->trapPositions, TRAPS) );
        labyrinth->treasuresPositions[i][0] = coords[0];
        labyrinth->treasuresPositions[i][1] = coords[1];
    }
}

void setRandomTrapsInLabyrinth(Labyrinth* labyrinth){
    for(int i = 0; i < TRAPS; i++){
        int coords[2];
        do{
            coords[0] = (rand() % (labyrinth->rows / 2)) * 2 + 1;
            coords[1] = (rand() % (labyrinth->columns / 2)) * 2 + 1;
        }while( isSameCoords(coords, labyrinth->keyPosition) 
        || isCoordinateInCoordinatesArray(coords, labyrinth->treasuresPositions, TREASURES) 
    || isCoordinateInCoordinatesArray(coords, (int**)labyrinth->trapPositions, i) );
        labyrinth->trapPositions[i][0] = coords[0];
        labyrinth->trapPositions[i][1] = coords[1];
    }
}

void buildLabyrinthBasys(Labyrinth* labyrinth){
    for(int i = 0; i < labyrinth->rows; i++){
        for(int j = 0; j < labyrinth->columns; j++){
            if( i % 2 == 1 && j % 2 ==1 ) {
                labyrinth->map[i][j] = PATH; // Path
            } else {
                labyrinth->map[i][j] = WALL; // Wall
            }
        }
    }
    // Set entrance and exit
    labyrinth->map[0][1] = ENTRANCE; // Entrance
    labyrinth->map[labyrinth->rows - 1][labyrinth->columns - 2] = EXIT; // Exit
    
    //set random key position (not on entrance or exit or on player start)
    labyrinth->keyPosition[0] = (rand() % (labyrinth->rows / 2)) * 2 + 1;
    labyrinth->keyPosition[1] = (rand() % (labyrinth->columns / 2)) * 2 + 1;

    // set random treasures positions
    setRandomTreasuresInLabyrinth(labyrinth);

    // set random traps positions
    setRandomTrapsInLabyrinth(labyrinth);
}

void displaymapValues( int **mapvalues, Labyrinth* labyrinth){
    for(int i = 0; i < labyrinth->rows; i++){
        for(int j = 0; j < labyrinth->columns; j++){
            if(mapvalues){
                printf("%3d ", mapvalues[i][j]);
            }
            else {
                printf("#");
            }
        }
        printf("\n");
    }
}

void replaceAllInMapValues(int oldValue, int newValue, int **mapvalues, Labyrinth* labyrinth){
    for(int i = 0; i < labyrinth->rows; i++){
        for(int j = 0; j < labyrinth->columns; j++){
            if(mapvalues[i][j] == oldValue){
                mapvalues[i][j] = newValue;
            }
        }
    }
}

void getRandomRoom(int *row, int *col, Labyrinth* labyrinth){
    do {
        *row = (rand() % (labyrinth->rows / 2)) * 2 + 1;
        *col = (rand() % (labyrinth->columns / 2)) * 2 + 1;
    } while(*row <= 0 || *row >= labyrinth->rows - 1 || *col <= 0 || *col >= labyrinth->columns - 1);
}

bool isLabyrinthComplete(int **mapvalues, Labyrinth* labyrinth ){
    int firstValue = mapvalues[1][1];
    for(int i = 0; i < labyrinth->rows; i++){
        for(int j = 0; j < labyrinth->columns; j++){
            if( i % 2 == 1 && j % 2 ==1 ) {
                if(mapvalues[i][j] != firstValue){
                    return false;
                }
            }
        }
    }
    return true;
}

void tryToremoveRandomWallAtRoomCoords(int roomRow, int roomCol, int **mapvalues, Labyrinth* labyrinth){
    int direction = rand() % 4; 
    int wallRow = roomRow;
    int wallCol = roomCol;
    int nextRoomRow = roomRow;
    int nextRoomCol = roomCol;

    switch(direction){
        case 0: // Up
            wallRow -= 1;
            nextRoomRow -= 2;
            break;
        case 1: // Down
            wallRow += 1;
            nextRoomRow += 2;
            break;
        case 2: // Left
            wallCol -= 1;
            nextRoomCol -= 2;
            break;
        case 3: // Right
            wallCol += 1;
            nextRoomCol += 2;
            break;
    }
    if(nextRoomRow > 0 && nextRoomRow < labyrinth->rows-1 && nextRoomCol > 0 && nextRoomCol < labyrinth->columns-1
        && mapvalues[roomRow][roomCol] != mapvalues[nextRoomRow][nextRoomCol] ){
        if(mapvalues[roomRow][roomCol] != mapvalues[nextRoomRow][nextRoomCol]){
            labyrinth->map[wallRow][wallCol] = PATH; // Remove wall
            int oldValue = mapvalues[nextRoomRow][nextRoomCol];
            int newValue = mapvalues[roomRow][roomCol];
            mapvalues[wallRow][wallCol] = newValue; 
            replaceAllInMapValues(oldValue, newValue, mapvalues, labyrinth);
        }
    }
}

void createLabyrinth(Labyrinth* labyrinth){
    int count = 1;
    int **mapvalues = malloc(labyrinth->rows * sizeof(int*));
    for(int i = 0; i < labyrinth->rows; i++){
        mapvalues[i] = malloc(labyrinth->columns * sizeof(int));
    }

    for(int i = 0; i < labyrinth->rows; i++){
        for(int j = 0; j < labyrinth->columns; j++){
            if( i % 2 == 1 && j % 2 ==1 ) {
                mapvalues[i][j] = count++;
            } 
        }
    }
    // This function can be used to create a more complex labyrinth if needed
    buildLabyrinthBasys(labyrinth);
    int roomRow, roomCol;
    while (!isLabyrinthComplete((int **)mapvalues, labyrinth)){
        getRandomRoom(&roomRow, &roomCol, labyrinth);
        tryToremoveRandomWallAtRoomCoords(roomRow, roomCol, (int **)mapvalues, labyrinth);
    }

    for(int i = 0; i < labyrinth->rows; i++){
        free(mapvalues[i]);
    }
    free(mapvalues);
}