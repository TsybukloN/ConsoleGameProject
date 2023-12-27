#include "ConsoleGameProject.h"

//Main logic of game
int main() {
    srand(static_cast<unsigned>(time(0)));
    Witcher main;
    MSG msg;

    cout << R"(
    ##   ##    ##      ##              ###                                   ###
    ##   ##            ##               ##                                  ####
    ##   ##   ###     #####    ####     ##       ####    ######            ## ##
    ## # ##    ##      ##     ##  ##    #####   ##  ##    ##  ##          ##  ##
    #######    ##      ##     ##        ##  ##  ######    ##              #######
    ### ###    ##      ## ##  ##  ##    ##  ##  ##        ##                  ##
    ##   ##   ####      ###    ####    ###  ##   #####   ####                 ##  
    )" << '\n';
    
    cout << "Type something to start ...";
    _getch();

    system("cls");
    
    while (main.gameOn()) {
        main.drawGame();

        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_TIMER) {
                main.moveMonster();
            }
        }

        if (_kbhit()) {
            char input = _getch();
            main.moveHero(input);
        }
    }

    system("cls");

    cout << R"(
    ##   ##    ##       ##              ###                                    ###
    ##   ##             ##               ##                                   ####
    ##   ##   ###      #####    ####     ##       ####    ######             ## ##
    ## # ##    ##       ##     ##  ##    #####   ##  ##    ##  ##           ##  ##
    #######    ##       ##     ##        ##  ##  ######    ##               #######
    ### ###    ##       ## ##  ##  ##    ##  ##  ##        ##                   ##
    ##   ##   ####       ###    ####    ###  ##   #####   ####                  ##  
    )" << '\n';

     cout << "Thanks for gaming, we are looking you like it\n"
          << "Type something to continue ...";
     _getch();

    return 0;
}