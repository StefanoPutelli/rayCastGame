#include "include/server.h"
#include <iostream>

using namespace std;

unsigned char screen[WIDTH][HEIGHT][4];

void renderScreen(float* fov_array) {
    for (int i = 0; i < WIDTH; i++) {
        if (fov_array[i] == -1) {
            for (int l = 0; l < HEIGHT; l++) {
                screen[i][l][0] = 0;
                screen[i][l][1] = 0;
                screen[i][l][2] = 0;
                screen[i][l][3] = 255;
            }
        } else {
            int screenCenter = (int)(HEIGHT / 2);
            int halfHeight = (int)(screenCenter/fov_array[i]);
            int start = screenCenter - halfHeight;
            int end = screenCenter + halfHeight;
            for (int l = 0; l < HEIGHT; l++) {
                if (l < start || l > end) {
                    screen[i][l][0] = 0;
                    screen[i][l][1] = 0;
                    screen[i][l][2] = 0;
                    screen[i][l][3] = 255;
                } else {
                    if(fov_array[i] > 5){
                        screen[i][l][0] = 0;
                        screen[i][l][1] = 0;
                        screen[i][l][2] = 0;
                        screen[i][l][3] = 255;
                    }else if(fov_array[i] > 4){
                        screen[i][l][0] = 0;
                        screen[i][l][1] = 0;
                        screen[i][l][2] = 0;
                        screen[i][l][3] = 255;
                    }else if(fov_array[i] > 3){
                        screen[i][l][0] = 0;
                        screen[i][l][1] = 0;
                        screen[i][l][2] = 0;
                        screen[i][l][3] = 255;
                    }else if(fov_array[i] > 2){
                        screen[i][l][0] = 0;
                        screen[i][l][1] = 0;
                        screen[i][l][2] = 0;
                        screen[i][l][3] = 255;
                    }else if(fov_array[i] > 1){
                        screen[i][l][0] = 0;
                        screen[i][l][1] = 0;
                        screen[i][l][2] = 0;
                        screen[i][l][3] = 255;
                    }else{
                        screen[i][l][0] = 0;
                        screen[i][l][1] = 0;
                        screen[i][l][2] = 0;
                        screen[i][l][3] = 255;
                    }
                }
            }
        }
    }
}



