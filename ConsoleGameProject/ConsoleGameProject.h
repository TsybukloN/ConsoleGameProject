#ifndef CONSOLEGAMEPROJECT_H
#define CONSOLEGAMEPROJECT_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <conio.h>
#include <Windows.h>
#include "Maps.h" // Libriary with Maps

using namespace std;

// Direction definitions
#define UP 'w'
#define DOWN 's'
#define RIGHT 'd'
#define LEFT 'a'
#define HIT 'e'
#define HEAL 'q'

/*
* @brief Hero, class with hero's field
*
* @param x X coordinate of hero on the gamefield matrix
* @param y Y coordinate of hero on the gamefield matrix
* @param symbol Symbol of hero, which displays on the gamefield
* @param points Count of hited monsters
* @param lifes Count of lifes of hero
*/

struct Hero {
    int x;
    int y;
    char symbol;
    unsigned short points;
    unsigned short lifes;
};

/*
* @brief Monster, class with monster's field
*
* @param x X coordinate of monster on the gamefield matrix
* @param y Y coordinate of monster on the gamefield matrix
* @param symbol Symbol of monster, which displays on the gamefield
* @param dieSymbol Symbol of monster's troope
* @param randomHit Amount of hit, which monster can do until decrementing of hero's life
* @param actualRandomHit Actual amount of hit
*/

struct Monster {
    int x;
    int y;
    char symbol;
    char dieSymbol;
    const unsigned short randomHit;
    unsigned short actualRandomHit = 0;
};

/*
* @class Witcher
* @brief Main class for game
*/

class Witcher {
private:
    char gamefield[Maps::kScreenHeight][Maps::kScreenWidth];
    Hero hero;
    Monster monster;
    HANDLE consoleHandle;
    HWND console;
    UINT_PTR timerID;
    const time_t monsterUpdateTime;
    const unsigned short goalPoints;

    /*
    * @brief Writes a string into the game matrix at the specified coordinates
    *
    * @param str String to be written
    * @param x X coordinate in the matrix
    * @param y Y coordinate in the matrix
    */
    void WriteStringInMatrix(const string& str, const int& x, const int& y) {
        auto size = str.size();
        for (int i = 0; i < size; i++) {
            gamefield[y][x + i] = str[i];
        }
    }

    /*
    * @brief Deletes all 'X' symbols on the game map (monster die symbols)
    */
    void deleteXOnMap() {
        for (int i = 0; i < Maps::kScreenHeight; i++) {
            for (int j = 0; j < Maps::kScreenWidth; j++) {
                if (gamefield[i][j] == monster.dieSymbol) {
                    gamefield[i][j] = Maps::emptyCell;
                }
            }
        }
    }

public:
    /*
    * @brief Constructor for the Witcher class
    */
    Witcher() : hero{ 1, Maps::infolineCount + 1, 'W', 0, 3 }, monster{ Maps::kScreenWidth / 2, Maps::kScreenHeight / 2, 'M', 'X', 3 },
        monsterUpdateTime(300), goalPoints(25)
    {
        // Seed the random number generator with the current time
        srand(static_cast<unsigned>(time(0)));

        Maps::Init(gamefield, Maps::tmp3);

        gamefield[hero.y][hero.x] = hero.symbol;
        gamefield[monster.y][monster.x] = monster.symbol;

        consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        console = GetConsoleWindow();
        RECT r;
        GetWindowRect(console, &r); // Get the current window size
        MoveWindow(console, r.left, r.top, 700, 600, TRUE); // Set new window size

        timerID = SetTimer(NULL, 0, monsterUpdateTime, (TIMERPROC)NULL);
    }

    /*
    * @brief Destructor for the Witcher class
    */
    ~Witcher() {
        KillTimer(NULL, timerID);
    }

    /*
    * @brief Checks if the game is still running.
    *
    * @return True if the hero has remaining lives, false otherwise.
    */
    bool hpControle() {
        return hero.lifes > 0;
    }

    /*
    * @brief Checks if the game goal is achieved.
    *
    * @return True if the hero has reached the goal points, false otherwise.
    */
    bool goalControle() {
        return hero.points < goalPoints;
    }

    /*
    * @brief Main game mode where the hero interacts with the monsters.
    */
    void mainMode() {
        while (this->hpControle() && this->goalControle()) {
            // Draw the game on the console
            this->drawGame();

            // Process Windows messages and move the monster on a timer
            MSG msg;
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_TIMER) {
                    this->moveMonster();
                }
            }

            // Check for user input
            if (_kbhit()) {
                char input = _getch();
                this->moveHero(input);
            }
        }

        system("cls");

        if (!this->hpControle()) {
            cout << R"(
  ________                                                  
 /  _____/_____    _____   ____     _______  __ ___________ 
/   \  ___\__  \  /     \_/ __ \   /  _ \  \/ // __ \_  __ \
\    \_\  \/ __ \|  Y Y  \  ___/  (  <_> )   /\  ___/|  | \/
 \______  (____  /__|_|  /\___  >  \____/ \_/  \___  >__|   
        \/     \/      \/     \/                   \/       
            )" << '\n';
        }

        if (!this->goalControle()) {
            cout << R"(
  ________                  .___                               
 /  _____/  ____   ____   __| _/    _________    _____   ____  
/   \  ___ /  _ \ /  _ \ / __ |    / ___\__  \  /     \_/ __ \ 
\    \_\  (  <_> |  <_> ) /_/ |   / /_/  > __ \|  Y Y  \  ___/ 
 \______  /\____/ \____/\____ |   \___  (____  /__|_|  /\___  >
        \/                   \/  /_____/     \/      \/     \/       
            )" << '\n';
        }
    }

    /*
    * @brief Displays the game menu and allows the user to start the game or view controls.
    */
    void showGame() {
        string menuOption;

        // Display the game title
        cout << R"(
/  \    /  \__|/  |_  ____ |  |__   ___________    /  |  | 
\   \/\/   /  \   __\/ ___\|  |  \_/ __ \_  __ \  /   |  |_
 \        /|  ||  | \  \___|   Y  \  ___/|  | \/ /    ^   /
  \__/\  / |__||__|  \___  >___|  /\___  >__|    \____   | 
       \/                \/     \/     \/             |__|
    )" << '\n';

        // Prompt the user to start the game
        do {
            cout << "======= Menu ======\n";
            cout << "1. Start game\n";
            cout << "2. Show game control\n";
            cout << "3. Exit\n";
            cout << "===================\n";

            cin >> menuOption;

            system("cls");

            if (menuOption == "1") {
                this->mainMode();
            }
            else if (menuOption == "2") {
                cout << "W S A D to Move, E to Hit and Q to Heal.\n"
                    << "If you need more lifes - go to the tavern ('+') and get it. 10 monsters is 1 life.\n"
                    << "The goal of game - 25 kills.\n";
                for (int i = 0;i < Maps::kScreenHeight - Maps::infolineCount;i++) {
                    for (int j = 0;j  < Maps::kScreenWidth;j++) {
                        cout << Maps::example[i][j];
                    }
                }
                cout << "\nEnter someting to exit...";
                _getch();

                system("cls");
            }
            else if (menuOption == "3") {
                cout << "Thank you for gaming. Good bay!" << endl;
            }
            else {
                cout << "Incorrect input. Please, try again ..." << endl;
            }

        } while (menuOption != "3");
    }

    /*
    * @brief Draws the game on the console window
    */
    void drawGame() {
        CHAR_INFO buffer[Maps::kScreenWidth * Maps::kScreenHeight]{};

        WriteStringInMatrix("In " + string(1, hero.symbol) + "' bag " + to_string(hero.points) + ((hero.points != 1) ? " monsters" : " monster") + " and " + to_string(hero.lifes) + " lifes", 0, 0);

        for (int y = 0; y < Maps::kScreenHeight; ++y) {
            for (int x = 0; x < Maps::kScreenWidth; ++x) {
                CHAR_INFO& cell = buffer[y * Maps::kScreenWidth + x];
                cell.Char.UnicodeChar = gamefield[y][x];
                if (gamefield[y][x] == hero.symbol) {
                    cell.Attributes = FOREGROUND_GREEN | FOREGROUND_RED;
                }
                else if (gamefield[y][x] == monster.dieSymbol) {
                    cell.Attributes = FOREGROUND_BLUE;
                }
                else if (gamefield[y][x] == '+') {
                    cell.Attributes = FOREGROUND_GREEN;
                }
                else if (gamefield[y][x] == monster.symbol) {
                    cell.Attributes = FOREGROUND_RED;
                }
                else {
                    cell.Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
                }
            }
        }

        COORD bufferSize = { static_cast<SHORT>(Maps::kScreenWidth), static_cast<SHORT>(Maps::kScreenHeight) };
        COORD characterPos = { 0, 0 };
        SMALL_RECT consoleWriteArea = { 0, 0, static_cast<SHORT>(Maps::kScreenWidth - 1), static_cast<SHORT>(Maps::kScreenHeight - 1) };
        WriteConsoleOutput(consoleHandle, buffer, bufferSize, characterPos, &consoleWriteArea);
    }

    /*
    * @brief Moves the monster on the game map and checks for hero collisions
    */
    void moveMonster() {
        int direction = rand() % 4;

        switch (direction) {
        case 0:
            if (gamefield[monster.y - 1][monster.x] == Maps::emptyCell) {
                gamefield[monster.y][monster.x] = Maps::emptyCell;
                gamefield[--monster.y][monster.x] = monster.symbol;
            }
            break;
        case 1:
            if (gamefield[monster.y + 1][monster.x] == Maps::emptyCell) {
                gamefield[monster.y][monster.x] = Maps::emptyCell;
                gamefield[++monster.y][monster.x] = monster.symbol;
            }
            break;
        case 2:
            if (gamefield[monster.y][monster.x - 1] == Maps::emptyCell) {
                gamefield[monster.y][monster.x] = Maps::emptyCell;
                gamefield[monster.y][--monster.x] = monster.symbol;
            }
            break;
        case 3:
            if (gamefield[monster.y][monster.x + 1] == Maps::emptyCell) {
                gamefield[monster.y][monster.x] = Maps::emptyCell;
                gamefield[monster.y][++monster.x] = monster.symbol;
            }
            break;
        }

        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                if (gamefield[monster.y + i][monster.x + j] == hero.symbol) {
                    monster.actualRandomHit++;
                    if (monster.actualRandomHit == monster.randomHit) {
                        hero.lifes--;
                        monster.actualRandomHit = 0;
                    }
                }
            }
        }
    }

    /*
    * @brief Moves the hero on the game map based on user input
    *
    * @param direction Direction of movement (UP, DOWN, LEFT, RIGHT, HIT, HEAL)
    */
    void moveHero(const char& direction) {
        switch (tolower(direction)) {
        case UP:
            if (gamefield[hero.y - 1][hero.x] == Maps::emptyCell) {
                gamefield[hero.y][hero.x] = Maps::emptyCell;
                gamefield[--hero.y][hero.x] = hero.symbol;
            }
            break;
        case DOWN:
            if (gamefield[hero.y + 1][hero.x] == Maps::emptyCell) {
                gamefield[hero.y][hero.x] = Maps::emptyCell;
                gamefield[++hero.y][hero.x] = hero.symbol;
            }
            break;
        case LEFT:
            if (gamefield[hero.y][hero.x - 1] == Maps::emptyCell) {
                gamefield[hero.y][hero.x] = Maps::emptyCell;
                gamefield[hero.y][--hero.x] = hero.symbol;
            }
            break;
        case RIGHT:
            if (gamefield[hero.y][hero.x + 1] == Maps::emptyCell) {
                gamefield[hero.y][hero.x] = Maps::emptyCell;
                gamefield[hero.y][++hero.x] = hero.symbol;
            }
            break;
        case HIT:
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (gamefield[hero.y + i][hero.x + j] == monster.symbol) {
                        hero.points++;
                        gamefield[monster.y][monster.x] = monster.dieSymbol;
                        do {
                            monster.x = rand() % (Maps::kScreenWidth - Maps::addCol - 4) + 1;
                            monster.y = rand() % (Maps::kScreenHeight - Maps::infolineCount - 2) + Maps::infolineCount + 1;
                        } while (gamefield[monster.y][monster.x] != Maps::emptyCell);
                        this->moveMonster();
                        monster.actualRandomHit = 0;
                        return;
                    }
                }
            }
            break;
        case HEAL:
            for (int i = -2; i <= 2; i++) {
                for (int j = -2; j <= 2; j++) {
                    if (gamefield[hero.y + i][hero.x + j] == Maps::symbolHeal) {
                        if (hero.points >= 10) {
                            hero.lifes++;
                            hero.points -= 10;
                            deleteXOnMap();
                        }
                        return;
                    }
                }
            }
            break;
        }
    }
};

#endif // CONSOLEGAMEPROJECT_H
