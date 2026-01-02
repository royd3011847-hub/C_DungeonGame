#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#define MAP_SIZE 10
#define STARTING_ROW 0
#define STARTING_COL 0

typedef enum {
    EMPTY,
    PLAYER,
    monster,
    VISITED
} thing;

typedef struct Creature{
    thing t;
    int hp;
    int damage;
} Creature;
Creature (*createMap())[MAP_SIZE];
void printMap(Creature (*map)[MAP_SIZE]);
int moveUp(Creature (*map)[MAP_SIZE], int row, int col);
int moveDown(Creature (*map)[MAP_SIZE], int row, int col);
int moveLeft(Creature (*map)[MAP_SIZE], int row, int col);
int moveRight(Creature (*map)[MAP_SIZE], int row, int col);