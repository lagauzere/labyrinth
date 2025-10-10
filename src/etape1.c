#include <stdio.h>
#include <stdlib.h>
#include "etape1.h"
#include <stdbool.h>
#include <locale.h>

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
    printf("test\n");
    for(int i = 0; i < labyrinthe->rows; i++){
        for(int j = 0; j < labyrinthe->columns; j++){
            if (i == labyrinthe->playerPosition[0] && j == labyrinthe->playerPosition[1]) {
                printf("%c", PLAYER); 
            }else if (labyrinthe->map[i][j] == WALL) {
                printf("%s", UTF8_WALL); 
            }else {
                printf("%c", labyrinthe->map[i][j]);
            }
        }
        printf("\n");
    }
}

void buildLabyrinthBasys(Labyrinthe* labyrinthe){
    for(int i = 0; i < labyrinthe->rows; i++){
        for(int j = 0; j < labyrinthe->columns; j++){
            if( i % 2 == 1 && j % 2 ==1 ) {
                labyrinthe->map[i][j] = PATH; // Path
            } else {
                labyrinthe->map[i][j] = WALL; // Wall
            }
        }
    }
    // Set entrance and exit
    labyrinthe->map[0][1] = PLAYER; // Entrance
    labyrinthe->map[labyrinthe->rows - 1][labyrinthe->columns - 2] = EXIT; // Exit
}

void displaymapValues( int **mapvalues, Labyrinthe* labyrinthe){
    for(int i = 0; i < labyrinthe->rows; i++){
        for(int j = 0; j < labyrinthe->columns; j++){
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

void replaceAllInMapValues(int oldValue, int newValue, int **mapvalues, Labyrinthe* labyrinthe){
    for(int i = 0; i < labyrinthe->rows; i++){
        for(int j = 0; j < labyrinthe->columns; j++){
            if(mapvalues[i][j] == oldValue){
                mapvalues[i][j] = newValue;
            }
        }
    }
}

void getRandomRoom(int *row, int *col, Labyrinthe* labyrinthe){
    do {
        *row = (rand() % (labyrinthe->rows / 2)) * 2 + 1;
        *col = (rand() % (labyrinthe->columns / 2)) * 2 + 1;
    } while(*row <= 0 || *row >= labyrinthe->rows - 1 || *col <= 0 || *col >= labyrinthe->columns - 1);
}

bool isLabyrinthComplete(int **mapvalues, Labyrinthe* labyrinthe ){
    int firstValue = mapvalues[1][1];
    for(int i = 0; i < labyrinthe->rows; i++){
        for(int j = 0; j < labyrinthe->columns; j++){
            if( i % 2 == 1 && j % 2 ==1 ) {
                if(mapvalues[i][j] != firstValue){
                    return false;
                }
            }
        }
    }
    return true;
}

void tryToremoveRandomWallAtRoomCoords(int roomRow, int roomCol, int **mapvalues, Labyrinthe* labyrinthe){
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
    if(nextRoomRow > 0 && nextRoomRow < labyrinthe->rows-1 && nextRoomCol > 0 && nextRoomCol < labyrinthe->columns-1
        && mapvalues[roomRow][roomCol] != mapvalues[nextRoomRow][nextRoomCol] ){
        if(mapvalues[roomRow][roomCol] != mapvalues[nextRoomRow][nextRoomCol]){
            labyrinthe->map[wallRow][wallCol] = PATH; // Remove wall
            int oldValue = mapvalues[nextRoomRow][nextRoomCol];
            int newValue = mapvalues[roomRow][roomCol];
            mapvalues[wallRow][wallCol] = newValue; 
            replaceAllInMapValues(oldValue, newValue, mapvalues, labyrinthe);
        }
    }

    
}

void createLabyrinthe(Labyrinthe* labyrinthe){
    int count = 1;
    int **mapvalues = malloc(labyrinthe->rows * sizeof(int*));
    for(int i = 0; i < labyrinthe->rows; i++){
        mapvalues[i] = malloc(labyrinthe->columns * sizeof(int));
    }

    for(int i = 0; i < labyrinthe->rows; i++){
        for(int j = 0; j < labyrinthe->columns; j++){
            if( i % 2 == 1 && j % 2 ==1 ) {
                mapvalues[i][j] = count++;
            } 
        }
    }
    // This function can be used to create a more complex labyrinth if needed
    buildLabyrinthBasys(labyrinthe);
    int roomRow, roomCol;
    while (!isLabyrinthComplete((int **)mapvalues, labyrinthe)){
        getRandomRoom(&roomRow, &roomCol, labyrinthe);
        tryToremoveRandomWallAtRoomCoords(roomRow, roomCol, (int **)mapvalues, labyrinthe);
    }

    for(int i = 0; i < labyrinthe->rows; i++){
        free(mapvalues[i]);
    }
    free(mapvalues);
}