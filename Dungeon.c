#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include "Dungeon.h"

Creature (*createMap())[MAP_SIZE]{

    Creature (*map)[MAP_SIZE] = malloc(MAP_SIZE * sizeof(Creature[MAP_SIZE]));

    if (!map) {
        return NULL;
    }
    for(int r=0; r<MAP_SIZE; r++){
        for(int c=0; c<MAP_SIZE; c++){
            map[r][c].t = EMPTY;
        }
    }
    
    map[STARTING_ROW][STARTING_COL].t = PLAYER;
    map[STARTING_ROW][STARTING_COL].hp = 50;
    map[STARTING_ROW][STARTING_COL].damage = 5;

    int count = 0;
    //monsters set so on average every 5th square is a monster
    int monsterCount = (MAP_SIZE * MAP_SIZE)/5;
    while(count < monsterCount){
        int row = rand()%MAP_SIZE;
        int col = rand()%MAP_SIZE;
        if(map[row][col].t==EMPTY && !(row == MAP_SIZE-1 && col == MAP_SIZE-1)){
            map[row][col].t = monster;
            map[row][col].hp = rand()%8+8;
            map[row][col].damage = 4;
            count++;
        }
    }
    
    return map;
}

void printMap(Creature (*map)[MAP_SIZE]){
    for (int i = 0; i < MAP_SIZE; i++){
        for (int k = 0; k < MAP_SIZE; k++){
            if(map[i][k].t == PLAYER){
                printf("\033[34m◈\033[0m");  // Blue
            }
            else if(map[i][k].t == monster){
                printf("\033[31m※\033[0m");  // Red
            }
            else if (i == MAP_SIZE-1 && k == MAP_SIZE-1){
                printf("\033[33m✭\033[0m");  // Yellow
            }
            else if (map[i][k].t == EMPTY){
                printf("o");  // Default color
            }
            else if (map[i][k].t == VISITED){
                printf("\033[32m⊗\033[0m");  // Green
            }
            printf(" ");
        }
        printf("\n");
    }
}


int moveUp(Creature (*map)[MAP_SIZE], int row, int col){
    if(map[row][col].t != PLAYER){
        printf("Error: wrong row or column\n");
        return 0;
    }
    if(row == 0){
        printf("Error, you ran into the top wall\n");
        return 0;
    }

    map[row-1][col] = map[row][col];
    map[row][col].t = VISITED;

    printf("\n");
    printMap(map);

    return 1;
}
int moveDown(Creature (*map)[MAP_SIZE], int row, int col){
    if(map[row][col].t != PLAYER){
        printf("Error: wrong row or column\n");
        return 0;
    }
    if(row == MAP_SIZE-1){
        printf("Error, you ran into the bottom wall\n");
        return 0;
    }
    map[row+1][col] = map[row][col];
    map[row][col].t = VISITED;

    printf("\n");
    printMap(map);

    return 1;
}
int moveLeft(Creature (*map)[MAP_SIZE], int row, int col){
    if(map[row][col].t != PLAYER){
        printf("Error: wrong row or column\n");
        return 0;
    }
    if(col == 0){
        printf("Error, you ran into the left wall\n");
        return 0;
    }
    map[row][col-1] = map[row][col];
    map[row][col].t = VISITED;

    printf("\n");
    printMap(map);

    return 1;
}
int moveRight(Creature (*map)[MAP_SIZE], int row, int col){
    if(map[row][col].t != PLAYER){
        printf("Error: wrong row or column\n");
        return 0;
    }
    if(col == MAP_SIZE-1){
        printf("Error, you ran into the right wall\n");
        return 0;
    }
    map[row][col+1] = map[row][col];
    map[row][col].t = VISITED;

    printf("\n");
    printMap(map);

    return 1;
}
