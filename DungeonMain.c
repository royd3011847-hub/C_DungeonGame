#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include "Dungeon.h"
#ifdef _WIN32
    #include <windows.h>
#endif

int main(){
    #ifdef _WIN32
        
        SetConsoleOutputCP(CP_UTF8);
    #endif
    int fair = 1;
    int Row = STARTING_ROW;
    int Col = STARTING_COL;
    srand(time(NULL));
    Creature (*map)[MAP_SIZE] = createMap();
    printMap(map);

    while (Row != MAP_SIZE - 1 || Col != MAP_SIZE - 1){
        char input;
        printf("What direction will you go? (\033[33mw, a, s, d\033[0m)\n");
        scanf(" %c", &input);
        input = tolower(input);

        switch (input) {
            case 'w':
                if(moveUp(map, Row, Col)){
                    Row--;
                }
                break;
            case 'a':
                if(moveLeft(map, Row, Col)){
                    Col--;
                }
                break;
            case 's':
                if(moveDown(map, Row, Col)){
                    Row++;
                }
                break;
            case 'd':
                if(moveRight(map, Row, Col)){
                    Col++;
                }
                break;
            case 'b':
                Row = MAP_SIZE-1;
                Col = MAP_SIZE-1;
                printf("Exited");
                fair = 0;
                break;
            default:
                printf("invalid input. Try again.\n");
                break;
            
        }
        int battleCheck = (checkForMonsters(map, Row, Col));
        if (battleCheck != (STARTING_HP + 1)){
            map[Row][Col].hp = battleCheck;
        }
        if(map[Row][Col].hp <= 0){
            break;
        }
        
        
    }
    if (fair && map[Row][Col].hp > 0){
        printf("You won! ");
        printf("Final HP: %d\n", map[Row][Col].hp);
        if(map[Row][Col].hp == STARTING_HP){
            printf("FLAWLESS!!");
        }
    }
    else{
        printf("You died! better luck next time :(");
    }
    return 0;
}