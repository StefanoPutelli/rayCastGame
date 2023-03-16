#ifndef __GAME_H__
#define __GAME_H__

#include <termios.h>
#include <unistd.h>

#include <math.h>
#include <iostream>

#include "world.h"

typedef struct dirVars {
    dirVars() {
        PDistInnerBlockX = ' ';
        PDistInnerBlockY = ' ';
    }
    dirVars(float _PDistInnerBlockX, float _PDistInnerBlockY) {
        PDistInnerBlockX = _PDistInnerBlockX;
        PDistInnerBlockY = _PDistInnerBlockY;
    }
    float PDistInnerBlockX;
    float PDistInnerBlockY;
} dirVars;

const int INT_MAX = 2147483647;

const int MAX_DEPTH = 20;

const float VIEW_STEP = 1;

const float tile_size = 1.0;

float getDecimals(float x);
void resetFovArray(int WIDTH, float* fov_array);
void resetMiniMap(unsigned char world_copy[Y][X]);
bool checkWolrd();
void saveInTheFov(int WIDTH, float* fov_array, int index, float dist);
dirVars getDirVars(int angle, float playerX, float playerY);
void markBlock(unsigned char world_copy[Y][X], int x, int y, unsigned char marker);
void rayCastInTheFov(int WIDTH, int FOV, int RESOLUTION, float* fov_array, unsigned char world_copy[Y][X], float direction, float playerX, float playerY);
void findAndSetPlayer(float* playerX, float* playerY);
void captureKey(float* direction, float* playerX, float* playerY);

char getch();

#endif