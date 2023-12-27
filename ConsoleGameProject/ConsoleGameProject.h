#pragma once

#ifndef CONSOLEGAMEPROJECT_H
#define CONSOLEGAMEPROJECT_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <conio.h>
#include <Windows.h>

using namespace std;

// Nessecary init area value
const int infoline_count = 1;
const int addCol = 3;
const int kScreenWidth = 50 + addCol;
const int kScreenHeight = 20 + infoline_count;

//Direction define
#define UP 'w'
#define DOWN 's'
#define RIGHT 'd'
#define LEFT 'a'
#define HIT 'e'
#define HEAL 'q'

//Game map
char gamefield[][kScreenWidth] = {
    "                                                   ",
    "################################################   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              ####",
    "#                                               | #",
    "#                                               |+#",
    "#                                               | #",
    "#                                              ####",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "#                                              #   ",
    "################################################   "
};

/*
* @brief Hero, class with hero's field  
* 
* @param x X coordinate of hero on the gamefield matrix
* @param y Y coordinate of hero on the gamefield matrix
* @param symbol Symbol of hero, which displaies on the gamefield
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
* @param symbol Symbol of monster, which displaies on the gamefield
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
    Hero hero;
    Monster monster;
    const char symbolHeal;
    const char emptyCell;
    HANDLE consoleHandle;
    HWND console;
    UINT_PTR timerID;
    const time_t monsterUpdateTime;
    const unsigned short goalPoints;

    void WriteStringInMatrix(const string& str, const int& x, const int& y) {
        auto size = str.size();
        for (int i = 0; i < size; i++) {
            gamefield[y][x + i] = str[i];
        }
    }

    void deleteXOnMap() {
        for (int i = 0; i < kScreenHeight; i++) {
            for (int j = 0; j < kScreenWidth; j++) {
                if (gamefield[i][j] == monster.dieSymbol) {
                    gamefield[i][j] = emptyCell;
                }
            }
        }
    }
public:
    Witcher() : symbolHeal('+'), emptyCell(' '), hero{1, infoline_count + 1, 'W', 0, 3}, monster{kScreenWidth / 2, kScreenHeight / 2, 'M', 'X', 3},
                monsterUpdateTime(300), goalPoints(25) 
    {
        gamefield[hero.y][hero.x] = hero.symbol;
        gamefield[monster.y][monster.x] = monster.symbol;

        consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        console = GetConsoleWindow();
        RECT r;
        GetWindowRect(console, &r); // Получение текущих размеров окна
        MoveWindow(console, r.left, r.top, 800, 600, TRUE); // Установка новых размеров

        timerID = SetTimer(NULL, 0, monsterUpdateTime, (TIMERPROC)NULL);
    }

    ~Witcher() {
        KillTimer(NULL, timerID);
    }

    bool gameOn() {
        return hero.lifes > 0 && hero.points < goalPoints;
    }

    void drawGame() {
        CHAR_INFO buffer[kScreenWidth * kScreenHeight];

        WriteStringInMatrix("In " + string(1, hero.symbol) + "' bag " + to_string(hero.points) + ((hero.points != 1) ? " monstors" : " monstor") + " and " + to_string(hero.lifes) + " lifes", 0, 0);

        for (int y = 0; y < kScreenHeight; ++y) {
            for (int x = 0; x < kScreenWidth; ++x) {
                CHAR_INFO& cell = buffer[y * kScreenWidth + x];
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

        COORD bufferSize = { static_cast<SHORT>(kScreenWidth), static_cast<SHORT>(kScreenHeight) };
        COORD characterPos = { 0, 0 };
        SMALL_RECT consoleWriteArea = { 0, 0, static_cast<SHORT>(kScreenWidth - 1), static_cast<SHORT>(kScreenHeight - 1) };
        WriteConsoleOutput(consoleHandle, buffer, bufferSize, characterPos, &consoleWriteArea);
    }

    void moveMonster() {
        int direction = rand() % 4;

        switch (direction) {
        case 0:
            if (gamefield[monster.y - 1][monster.x] == emptyCell) {
                gamefield[monster.y][monster.x] = emptyCell;
                gamefield[--monster.y][monster.x] = monster.symbol;
                break;
            }
        case 1:
            if (gamefield[monster.y + 1][monster.x] == emptyCell) {
                gamefield[monster.y][monster.x] = emptyCell;
                gamefield[++monster.y][monster.x] = monster.symbol;
                break;
            }
        case 2:
            if (gamefield[monster.y][monster.x - 1] == emptyCell) {
                gamefield[monster.y][monster.x] = emptyCell;
                gamefield[monster.y][--monster.x] = monster.symbol;
                break;
            }
        case 3:
            if (gamefield[monster.y][monster.x + 1] == emptyCell) {
                gamefield[monster.y][monster.x] = emptyCell;
                gamefield[monster.y][++monster.x] = monster.symbol;
                break;
            }
        }

        for (int i = -1;i <= 1;i++) {
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

    void moveHero(const char& direction) {
        switch (tolower(direction)) {
        case UP:
            if (gamefield[hero.y - 1][hero.x] == emptyCell) {
                gamefield[hero.y][hero.x] = emptyCell;
                gamefield[--hero.y][hero.x] = hero.symbol;
            }
            break;
        case DOWN:
            if (gamefield[hero.y + 1][hero.x] == emptyCell) {
                gamefield[hero.y][hero.x] = emptyCell;
                gamefield[++hero.y][hero.x] = hero.symbol;
            }
            break;
        case LEFT:
            if (gamefield[hero.y][hero.x - 1] == emptyCell) {
                gamefield[hero.y][hero.x] = emptyCell;
                gamefield[hero.y][--hero.x] = hero.symbol;
            }
            break;
        case RIGHT:
            if (gamefield[hero.y][hero.x + 1] == emptyCell) {
                gamefield[hero.y][hero.x] = emptyCell;
                gamefield[hero.y][++hero.x] = hero.symbol;
            }
            break;
        case HIT:
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (gamefield[hero.y + i][hero.x + j] == monster.symbol) {
                        hero.points++;
                        gamefield[monster.y][monster.x] = monster.dieSymbol;
                        monster.x = rand() % (kScreenWidth - addCol - 3) + 1;
                        monster.y = rand() % (kScreenHeight - infoline_count - 2) + infoline_count + 1;
                        this->moveMonster();
                        monster.actualRandomHit = 0;
                        return;
                    }
                }
            }
            break;
        case HEAL:
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    if (gamefield[hero.y + i][hero.x + j] == '|') {
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
