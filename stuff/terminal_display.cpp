#include "include/game.h"

using namespace std;

int screen[WIDTH][HEIGHT];

void printFovArray(float* fov_array) {
    for (int i = 0; i < WIDTH; i++) {
            cout << fov_array[i] << " ";
    }
    cout << endl;
}

void renderScreen(float* fov_array) {
    for (int i = 0; i < WIDTH; i++) {
        if (fov_array[i] == -1) {
            for (int l = 0; l < HEIGHT; l++) {
                screen[i][l] = ' ';
            }
        } else {
            int screenCenter = (int)(HEIGHT / 2);
            int halfHeight = (int)(screenCenter/fov_array[i]);
            int start = screenCenter - halfHeight;
            int end = screenCenter + halfHeight;
            for (int l = 0; l < HEIGHT; l++) {
                if (l < start || l > end) {
                    screen[i][l] = 0;
                } else {
                    if(fov_array[i] > 5){
                        screen[i][l] = 6;
                    }else if(fov_array[i] > 4){
                        screen[i][l] = 5;
                    }else if(fov_array[i] > 3){
                        screen[i][l] = 4;
                    }else if(fov_array[i] > 2){
                        screen[i][l] = 3;
                    }else if(fov_array[i] > 1){
                        screen[i][l] = 2;
                    }else{
                        screen[i][l] = 1;
                    }
                }
            }
        }
    }
}

void printScreen() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if(screen[j][i] == 0){
                cout << "  ";
            }else if(screen[j][i] == 7){
                cout << "░░";
            }else if(screen[j][i] == 6){
                cout << "▒░" ;
            }else if(screen[j][i] == 5){
                cout << "▒▒";
            }else if(screen[j][i] == 4){
                cout << "▓▒";
            }else if(screen[j][i] == 3){
                cout << "▓▓";
            }else if(screen[j][i] == 2){
                cout << "█▓";
            }else if(screen[j][i] == 1){
                cout << "██";
            }
        }
        cout << endl;
    }
}

void printMiniMap(int world_copy[Y][X], float playerX, float playerY) {
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            if (world_copy[i][j] == 1) {
                cout << "█";
            } else if (i == (int)playerY && j == (int)playerX) {
                cout << "P";
            } else if (world_copy[i][j] == 3) {
                cout << "X";
            } else if (world_copy[i][j] == 4){
                cout << "Y";
            }else {
                cout << " ";
            }
        }
        cout << endl;
    }
}