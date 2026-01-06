#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include "Dungeon.h"
#include <windows.h>

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

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
    map[STARTING_ROW][STARTING_COL].hp = STARTING_HP;
    map[STARTING_ROW][STARTING_COL].damage = 5;

    int count = 0;
    //monsters set so on average every 4th square is a monster
    int monsterCount = (MAP_SIZE * MAP_SIZE)/4;
    while(count < monsterCount){
        int row = rand()%MAP_SIZE;
        int col = rand()%MAP_SIZE;
        if(map[row][col].t==EMPTY && !(row == MAP_SIZE-1 && col == MAP_SIZE-1) && !(row == 0 && col == 1) && !(row == 1 && col == 0)){
            map[row][col].t = monster;
            map[row][col].hp = rand()%8+8;
            map[row][col].damage = rand()%3+4;
            count++;
        }
    }
    
    return map;
}

void printMap(Creature (*map)[MAP_SIZE]){
    clearScreen();
    for (int i = 0; i < MAP_SIZE; i++){
        for (int k = 0; k < MAP_SIZE; k++){
            // Blue fpr player
            if(map[i][k].t == PLAYER){
                printf("\033[34m◈\033[0m");  
            }
            // Red for monster
            else if(map[i][k].t == monster){
                printf("\033[31m※\033[0m");  
            }
            // Yellow for goal
            else if (i == MAP_SIZE-1 && k == MAP_SIZE-1){
                printf("\033[33m✭\033[0m");  
            }
            //default colorfor empty
            else if (map[i][k].t == EMPTY){
                printf("o");
            }
            // Green for visited
            else if (map[i][k].t == VISITED){
                printf("\033[32m⊗\033[0m"); 
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
        printf("Error: you ran into the top wall\n");
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
        printf("Error: you ran into the bottom wall\n");
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
        printf("Error: you ran into the left wall\n");
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
        printf("Error: you ran into the right wall\n");
        return 0;
    }
    map[row][col+1] = map[row][col];
    map[row][col].t = VISITED;

    printf("\n");
    printMap(map);

    return 1;
}

int checkForMonsters(Creature (*map)[MAP_SIZE], int row, int col){
    //printf("Func Ran\n");
    Creature arr[3];
    int monCount = 0;
    if(map[row-1][col].t == monster && row != 0){
        arr[monCount] = map[row-1][col];
        monCount++;
    }
    if(map[row+1][col].t == monster && row != MAP_SIZE-1){
        arr[monCount] = map[row+1][col];
        monCount++;
    }
    if(map[row][col-1].t == monster && col != 0){
        arr[monCount] = map[row][col-1];
        monCount++;
    }
    if(map[row][col+1].t == monster && col != MAP_SIZE-1){
        arr[monCount] = map[row][col+1];
        monCount++;
    }
    if (!monCount){
        return STARTING_HP+1;
    }
    //printf("Right before battle\n");
    int a = battle(arr, monCount, map[row][col]);
    if(a <= 0){
        return a; //lose
    }
    else{
        printf("You won the battle!\n");
        if(map[row-1][col].t == monster){
            map[row-1][col].t = EMPTY;
        }
        if(map[row+1][col].t == monster){
            map[row+1][col].t = EMPTY;
        }
        if(map[row][col-1].t == monster){
            map[row][col-1].t = EMPTY;
        }
        if(map[row][col+1].t == monster){
            map[row][col+1].t = EMPTY;
        }
        //one in 4 chance per monster
        //player gets healed
        int healed = 0;
        for (int i = 0; i < monCount; i++){
            if(!(rand()%4)){
                printf("A monster was carrying a \033[31mHealth Potion\033[0m!\n");
                if(map[row][col].hp >= STARTING_HP){
                    printf("the potion doesn't do anything... you already are at max HP.\n");
                }
                else{
                    int healAmount = rand()%3+3;
                    healed = 1;
                    map[row][col].hp += healAmount;
                    printf("You heal \033[32m%d\033[0m HP!\n", healAmount);
                    if(map[row][col].hp > STARTING_HP){
                        map[row][col].hp = STARTING_HP;
                    }
                }
            }
        }
        if(healed){
            printf("Your total health is now \033[32m%d\033[0m\n", map[row][col].hp);
        }

        
        printMap(map);
        return a;
    }

}

int battle(Creature arr[], int monCount, Creature player){
    //printf("Battle Init\n");
    
    char input;
    int critMultiplier = 2;
    int critDamage = (player.damage*critMultiplier);
    if (!monCount){
        printf("Error. func shouldn't have been called\n");
        return -100;
    }

    int monstersAlive = 1;

    if(monCount == 1){
        printf("A Monster has found you!\n");
        
    }
    else{
        printf("%d monsters have found you!\n", monCount);
        
    }

    while(monstersAlive){
        //player attack
        do{
            //shows stats after each round
            for (int i = 0; i < monCount; i++){
            if(arr[i].hp > 0){
                printf("Monster %d's HP: \033[31m%d\033[0m\n", i+1, arr[i].hp);
            }
            else{
                printf("Monster %d: \033[33mDEFEATED\033[0m\n", i+1);
            }
        }
        printf("Your HP: \033[32m%d\033[0m\n", player.hp);
            char inputHolder;
            printf("\nYour move! What will you do?\n");
            printf("Stab (\033[33m1\033[0m)      slash (\033[33m2\033[0m)      info (i)\n");
            scanf(" %c", &inputHolder);
            input = inputHolder;
            if(input == 'i'){
                printf("Stabbing does damage to the first enemy with a chance to crit\n");
                printf("Slashing does damage to all enamies\n");
            }
        } while(input != '1' && input != '2');
        //stab
        if(input == '1'){
            printf("You chose to stab!");
            int monsterAttacking = 0;
            while(arr[monsterAttacking].hp <= 0){
                monsterAttacking++;
            }
            if(rand()%2 == 1){
                printf(" It's a critical hit!\n");
                printf("You did \033[34m%d\033[0m damage!!\n", critDamage);
                arr[monsterAttacking].hp -= critDamage;
                
            }
            else{
                printf(" You did \033[34m%d\033[0m damage!\n", player.damage);
                arr[monsterAttacking].hp -= player.damage;
            }
            if(arr[monsterAttacking].hp<=0){
                printf("Monster %d was slain!\n", monsterAttacking+1);
            }
        }
        //slash
        else{
            printf("you chose to slash!");
            printf(" You did \033[34m%d\033[0m damage to all monsters!\n", player.damage);
            for (int i = 0; i < monCount; i++){
                arr[i].hp -= player.damage;
                if(arr[i].hp <= 0){
                    printf("Monster %d was slain!\n", i+1);
                }
            }

        }
        //check how many monsters are left alive
        int monsLeft = 0;
        for (int i = 0; i < monCount; i++){
            if(arr[i].hp > 0){
                monsLeft += 1;
            }
        }
        if(!monsLeft){
            return player.hp;
        }
        //monsters attack
        printf("\n");
        for (int i = 0; i < monCount; i++){
            if(arr[i].hp >= 0){
                printf("Monster %d attacks!\n", i+1);
                //3 in 5 chance of the monster hitting
                if(rand() % 5 < 3){
                    printf("He hits and does \033[31m%d\033[0m damage!", arr[i].damage);
                    player.hp -= arr[i].damage;
                    if(player.hp <= 0){
                        printf(" You died!!\n");
                        return player.hp;
                    }
                    else{
                        printf(" You have \033[32m%d\033[0m health left!\n", player.hp);
                    }
                }
                else{
                    printf("He misses!\n");
                }
            }
        }
    }

    return player.hp;
}

