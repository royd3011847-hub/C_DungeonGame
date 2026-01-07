
#include "Dungeon.h"
#ifdef _WIN32
    #include <windows.h>
#endif


int main(){
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif
    int fair = 1;
    int Row = 0;
    int Col = 0;
    srand(time(NULL));
    Creature (*map)[MAP_SIZE] = createMap();

    
    printMap(map, map[Row][Col].hp);

    while (Row != MAP_SIZE - 1 || Col != MAP_SIZE - 1){
        char input;
        printf("What direction will you go? (\033[33mw, a, s, d\033[0m), info (\033[33mi\033[0m)\n");
        //player input
        scanf(" %c", &input);
        input = tolower(input);
        if(input == 'b'){
            printf("Exited");
            return 0;
        }
        else{
            playerInput(map, &Row, &Col, input);
        }

        //check after player moves
        int battleCheck = (checkForMonsters(map, Row, Col));
        if (battleCheck != (STARTING_HP + 1)){
            map[Row][Col].hp = battleCheck;
        }
        if(map[Row][Col].hp <= 0){
            break;
        }
        
        
    }
    if (map[Row][Col].hp > 0){
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