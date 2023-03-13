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


const int FOV = 90;
const int WIDTH = 180;

const int RESOLUTION = WIDTH/FOV;
const int HEIGHT = 80;
const int MAX_DEPTH = 20;

const int X = 31;
const int Y = 21;

const int INT_MAX = 2147483647;

const int world[Y][X] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1},
    {1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
    {1,0,0,1,0,0,1,1,1,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1,0,0,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,1,1,1,0,0,1,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,1,1,1,1},
    {1,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
    {1,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1,0,0,1,1,1,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1,0,0,1,1,1,1,0,0,1},
    {1,0,0,1,0,0,0,2,0,0,0,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,0,0,1},
    {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,1,0,0,1},
    {1,0,0,1,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,0,1,1,1,1},
    {1,0,0,1,1,1,1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1},
    {1,1,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,0,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};


const float tile_size = 1.0;

float getDecimals(float x);
void resetFovArray();
void resetMiniMap();
bool checkWolrd();
void saveInTheFov(int index, float dist);
dirVars getDirVars(int angle);
void markBlock(int x, int y, int marker);
void rayCastInTheFov();
void printFovArray(float* fov_array);
void findAndSetPlayer();
void renderScreen(float* fov_array);
void printScreen();
void printMiniMap(int world_copy[Y][X], float playerX, float playerY);
void captureKey();
void run();

char getch();

#endif

