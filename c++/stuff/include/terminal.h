#ifndef __TERMINAL_H__
#define __TERMINAL_H__

#include "world.h"

void printFovArray(float* fov_array, int WIDTH);
void renderScreen(float* fov_array,unsigned char** screen, int WIDTH, int HEIGHT);
void printScreen(unsigned char** screen, float direction, float playerX, float playerY,int WIDTH, int HEIGHT);
void printMiniMap(unsigned char world_copy[Y][X], float playerX, float playerY);

#endif
