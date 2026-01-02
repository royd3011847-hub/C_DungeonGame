#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include "Dungeon.h"


int main(){
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
        
    }
    if (fair){
        printf("You won!!!");
    }
    return 0;
}