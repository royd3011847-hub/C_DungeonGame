#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#define MAP_SIZE 10
#define STARTING_HP 25

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

void clearScreen();
void printMap(Creature (*map)[MAP_SIZE], int hp);
void playerInput(Creature (*map)[MAP_SIZE], int *row, int *col, char input);
void printCurrentHP(int hp);
int moveUp(Creature (*map)[MAP_SIZE], int row, int col);
int moveDown(Creature (*map)[MAP_SIZE], int row, int col);
int moveLeft(Creature (*map)[MAP_SIZE], int row, int col);
int moveRight(Creature (*map)[MAP_SIZE], int row, int col);
int checkForMonsters(Creature (*map)[MAP_SIZE], int row, int col);
int battle(Creature arr[], int count, Creature player);
