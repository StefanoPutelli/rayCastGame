#include "include/game.h"

using namespace std;

void printFovArray(float* fov_array, int WIDTH) {
    for (int i = 0; i < WIDTH; i++) {
        cout << fov_array[i] << " ";
    }
    cout << endl;
}

void renderScreen(float* fov_array, unsigned char** screen, int WIDTH, int HEIGHT) {
    for (int i = 0; i < WIDTH; i++) {
        if (fov_array[i] == -1) {
            for (int l = 0; l < HEIGHT; l++) {
                screen[i][l] = ' ';
            }
        } else {
            int screenCenter = (int)(HEIGHT / 2);
            int halfHeight = (int)(screenCenter / fov_array[i]);
            int start = screenCenter - halfHeight;
            int end = screenCenter + halfHeight;
            for (int l = 0; l < HEIGHT; l++) {
                if (l < start || l > end) {
                    screen[i][l] = 0;
                } else {
                    if (fov_array[i] > 5) {
                        screen[i][l] = 6;
                    } else if (fov_array[i] > 4) {
                        screen[i][l] = 5;
                    } else if (fov_array[i] > 3) {
                        screen[i][l] = 4;
                    } else if (fov_array[i] > 2) {
                        screen[i][l] = 3;
                    } else if (fov_array[i] > 1) {
                        screen[i][l] = 2;
                    } else {
                        screen[i][l] = 1;
                    }
                }
            }
        }
    }
}

void printScreen(unsigned char** screen, float direction, float playerX, float playerY, int WIDTH, int HEIGHT) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (screen[j][i] == 0) {
                cout << "  ";
            } else if (screen[j][i] == 7) {
                cout << "░░";
            } else if (screen[j][i] == 6) {
                cout << "▒░";
            } else if (screen[j][i] == 5) {
                cout << "▒▒";
            } else if (screen[j][i] == 4) {
                cout << "▓▒";
            } else if (screen[j][i] == 3) {
                cout << "▓▓";
            } else if (screen[j][i] == 2) {
                cout << "█▓";
            } else if (screen[j][i] == 1) {
                cout << "██";
            }
        }
        cout << endl;
    }
    cout << "angle: " << direction << endl;
    cout << "x: " << playerX << " y: " << playerY << endl;
}

void printMiniMap(unsigned char world_copy[Y][X], float playerX, float playerY) {
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            if (world_copy[i][j] == '#') {
                cout << "█";
            } else if (i == (int)playerY && j == (int)playerX) {
                cout << "P";
            } else if (world_copy[i][j] == 'X') {
                cout << "X";
            } else if (world_copy[i][j] == 'Y') {
                cout << "Y";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
}