
#include "Dungeon.h"
#ifdef _WIN32
    #include <windows.h>
#endif

void clearScreen() {
    printf("\033[H\033[J");
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
    
    map[0][0].t = PLAYER;
    map[0][0].hp = STARTING_HP;
    map[0][0].damage = 5;

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

void printCurrentHP(int hp){
    hp <= 5 ? printf("Current HP: \033[31m%d\033[0m!\n", hp)
     : printf("Current HP: \033[32m%d\033[0m!\n", hp);
}

void printMap(Creature (*map)[MAP_SIZE], int hp){
    printCurrentHP(hp);
    for (int i = 0; i < MAP_SIZE; i++){
        for (int k = 0; k < MAP_SIZE; k++){
            // Blue for player
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

void playerInput(Creature (*map)[MAP_SIZE], int *row, int *col, char input){
    switch (input) {
        case 'w':
            if(moveUp(map, *row, *col)){
                (*row)--;
            }
            break;
        case 'a':
            if(moveLeft(map, *row, *col)){
                (*col)--;
            }
            break;
        case 's':
            if(moveDown(map, *row, *col)){
                (*row)++;
            }
            break;
        case 'd':
            if(moveRight(map, *row, *col)){
                (*col)++;
            }
            break;
        case 'i':
            printf("\033[33m--------------------------------------------\033[0m\n");
            printf("your goal is to make it to the star in the bottom right of the map\n");
            printf("You get there by using 'w' 'a' 's' 'd' to move up, down, left, and right\n");
            printf("The red monsters \033[31m※\033[0m are trying to kill you before you get there\n");
            printf("if you get within one tile of a monster (diagonals don't count), it will fight you\n");
            printf("if you are touching multiple monsters, you will fight multiple at once\n");
            printf("good luck making it before your HP runs out!!\n");
            printf("Input (b) to exit.\n");
            printf("\033[33m--------------------------------------------\033[0m\n");
            break;
        default:
            printf("invalid input. Try again.\n");
            break;
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
    clearScreen();
    printMap(map, map[row-1][col].hp);

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
    clearScreen();
    printMap(map, map[row+1][col].hp);

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
    clearScreen();
    printMap(map, map[row][col-1].hp);

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
    clearScreen();
    printMap(map, map[row][col+1].hp);

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
    printf("You won the battle!\n");
    map[row][col].hp = a;
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

    
    printMap(map, map[row][col].hp);
    return a;

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
        printCurrentHP(player.hp);
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

