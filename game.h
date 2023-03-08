#ifndef __GAME_H__
#define __GAME_H__

#include <termios.h>
#include <unistd.h>

#include <cmath>
#include <iostream>

#include <vector>

typedef struct dirVars {
    dirVars(){
        PDistInnerBlockX = 0;
        PDistInnerBlockY = 0;
    }
    dirVars(float _PDistInnerBlockX, float _PDistInnerBlockY) {
        PDistInnerBlockX = _PDistInnerBlockX;
        PDistInnerBlockY = _PDistInnerBlockY;
    }
    float PDistInnerBlockX;
    float PDistInnerBlockY;
} dirVars;

const int FOV = 60;
const int RESOLUTION = 3;
const int HEIGHT = 80;
const int WIDTH = FOV*RESOLUTION;

const int X = 31;
const int Y = 21;

const int INT_MAX = 2147483647;


const float tile_size = 1.0;

float getDecimals(float x);
void resetFovArray();
void resetMiniMap();
bool checkWolrd();
void saveInTheFov(int index, float dist);
dirVars getDirVars(int angle);
void markBlock(int x, int y, int marker);
void rayCastInTheFov(int depth);
void printFovArray();
void findAndSetPlayer();
void renderScreen();
void printScreen();
void printMiniMap();
void captureKey();

char getch();

#endif

