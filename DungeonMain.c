#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include "Dungeon.h"
#include <windows.h>


int main(){
    SetConsoleOutputCP(CP_UTF8);
    int fair = 1;
    int Row = STARTING_ROW;
    int Col = STARTING_COL;
    srand(time(NULL));
    Creature (*map)[MAP_SIZE] = createMap();
    printMap(map);

    while (Row != MAP_SIZE-1 || Col != MAP_SIZE-1){
        char input;
        printf("What direction will you go? (w, a, s, d)\n");
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
        if (battleCheck != 51){
            map[Row][Col].hp = battleCheck;
        }
        if(map[Row][Col].hp <= 0){
            break;
        }
        
        
    }
    if (fair && map[Row][Col].hp > 0){
        printf("You won! ");
        printf("Final HP: %d", map[Row][Col].hp);
    }
    else{
        printf("You died! better luck next time :(");
    }
    return 0;
}