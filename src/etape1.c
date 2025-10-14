#include <stdio.h>
#include <stdlib.h>
#include "etape1.h"
#include <stdbool.h>
#include <locale.h>

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
    return labyrinth;
}

void freeLabyrinth(Labyrinth* labyrinth){
    for(int i = 0; i < labyrinth->rows; i++){
        free(labyrinth->map[i]);
    }
    free(labyrinth->map);
}


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
                    printf("%c", PATH); // open door
                }
                else{
                    printf("%c", DOOR); // closed door
                }
            }
            else {
                printf("%c", labyrinth->map[i][j]);
            }
        }
        printf("\n");
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