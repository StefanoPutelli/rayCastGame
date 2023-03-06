#include <unistd.h>
#include <cmath>
#include <iostream>

using namespace std;

const int FOV = 181;
const int HEIGHT = 40;

typedef struct dirVars {
    dirVars(int _x, int _y, float _PDistInnerBlockX, float _PDistInnerBlockY) {
        x = _x;
        y = _y;
        PDistInnerBlockX = _PDistInnerBlockX;
        PDistInnerBlockY = _PDistInnerBlockY;
    }
    int x;
    int y;
    float PDistInnerBlockX;
    float PDistInnerBlockY;
} dirVars;

const int X = 11;
const int Y = 10;

// up -> 0
// right -> 1
// down -> 2
// left -> 3

// 0 -> player not found
// 1 -> world not valid
// 2 -> step to large
// 3 -> error in direction vars

// angle where the player is looking, 0 is left and 90 is up and 180 is right and 270 is down

const float tile_size = 1.0;

float playerX;
float playerY;

int step = 1;

int visibleBlock[10];

char screen[FOV][HEIGHT];

float fov_array[FOV];


int world[Y][X] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1}, {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1}, {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

float getDecimals(float x) { return x - (int)x; }

void resetFovArray(int len) {
    for (int i = 0; i < len; i++) {
        fov_array[i] = -1;
    }
}

bool checkWolrd() {
    for (int i = 0; i < Y; i++) {
        if (world[i][0] != 1 || world[i][X - 1] != 1) {
            return false;
        }
    }
    for (int i = 0; i < X; i++) {
        if (world[0][i] != 1 || world[Y - 1][i] != 1) {
            return false;
        }
    }
    return true;
}

void saveInTheFov(int index, float dist) { fov_array[index] = abs(dist); }

// returns:
// >0 -> collision on x
// <0 -> collision on y
// 0 -> both, on the angle
// null -> no collision

dirVars getDirVars(int angle) {
    if (angle < 90) {
        if (angle == 0) {
            return dirVars(1, 0, tile_size - getDecimals(playerX),
                           getDecimals(playerY));
        }
        return dirVars(1, -1, tile_size - getDecimals(playerX),
                       getDecimals(playerY));
    } else if (angle < 180) {
        if (angle == 90) {
            return dirVars(0, -1, getDecimals(playerX), getDecimals(playerY));
        }
        return dirVars(-1, -1, getDecimals(playerX), getDecimals(playerY));
    } else if (angle < 270) {
        if (angle == 180) {
            return dirVars(-1, 0, getDecimals(playerX),
                           tile_size - getDecimals(playerY));
        }
        return dirVars(-1, 1, getDecimals(playerX),
                       tile_size - getDecimals(playerY));
    } else {
        if (angle == 270) {
            return dirVars(0, 1, tile_size - getDecimals(playerX),
                           tile_size - getDecimals(playerY));
        }
        return dirVars(1, 1, tile_size - getDecimals(playerX),
                       tile_size - getDecimals(playerY));
    }
}

void rayCastInTheFov(int depth, int direction) {
    float x_distance;
    float y_distance;
    for (int i = 0; i < FOV; i++) {
        int angle = (direction - FOV / 2 + i) % 360;
        dirVars dirVal = getDirVars(angle);
        if (dirVal.x == 0 && dirVal.y == 0) {
            cerr << "Direction error";
            exit(3);
        }
        float angle_rad = (float)angle * 3.14159 / 180;
        for (int dpt = 0; dpt < depth; dpt++) {
            if (dirVal.y == 0) {
                y_distance = dirVal.PDistInnerBlockY + dpt * tile_size;
            } else {
                y_distance = (dirVal.PDistInnerBlockY + dpt * tile_size) *
                             dirVal.y / sin(angle_rad);
            }
            if (dirVal.x == 0) {
                x_distance = dirVal.PDistInnerBlockX + dpt * tile_size;
            } else {
                x_distance = (dirVal.PDistInnerBlockX + dpt * tile_size) *
                             dirVal.x / cos(angle_rad);
            }
            if (x_distance < y_distance) {
                int dY = (int)playerY;
                int dX =
                    (int)(x_distance + playerX + dirVal.x +
                          0.1 *
                              dirVal
                                  .x);  //we need that 0.1 to make the apporximation for checking if there is a wall or not
                if (world[dX][dY] == 1) {
                    saveInTheFov(i, x_distance);
                    break;
                }
            } else {
                int dX = (int)playerX;
                int dY =
                    (int)(y_distance + playerY + dirVal.y + 0.1 * dirVal.y);
                //   cout << "dx " << dX << " " << "dy " << dY << " py " << playerY << " dirvaly " << dirVal.y << " angle " << angle << endl;
                if (world[dX][dY] == 1) {
                    saveInTheFov(i, y_distance);
                    break;
                }
            }
        }
    }
}

void printFovArray() {
    for (int i = 0; i < FOV; i++) {
        cout << fov_array[i] << " ";
    }
    cout << endl;
}

void findAndSetPlayer() {
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            if (world[i][j] == 2) {
                playerX = j + tile_size / 2;
                playerY = i + tile_size / 2;
                return;
            }
        }
    }
    cout << "player not found";
    exit(0);
}

// else if (fov_array[i] > 5) {
//             cout << " ";
//         } else if (fov_array[i] > 4) {
//             cout << "░";
//         } else if (fov_array[i] > 3) {
//             cout << "▒";
//         } else if (fov_array[i] > 2) {
//             cout << "▒";
//         } else if (fov_array[i] > 1) {
//             cout << "▓";
//         } else if (fov_array[i] > 0) {
//             cout << "█";
//         }

void renderScreen() {
    for (int i = 0; i < FOV; i++) {
        if (fov_array[i] == -1) {
            for(int l = 0; l < HEIGHT; l++){
                screen[i][l] = ' ';
            }
        } else {
            int height = (int)(HEIGHT / fov_array[i]);
            int start = (HEIGHT - height) / 2;
            int end = start + height;
            for (int l = 0; l < HEIGHT; l++) {
                if (l < start || l > end) {
                    screen[i][l] = ' ';
                } else {
                    screen[i][l] = '#';
                }
            }
        }
    }
}

void printScreen(){
    for(int i = 0; i < HEIGHT; i++){
        for(int j = 0; j < FOV; j++){
            cout << screen[j][i];
        }
        cout << endl;
    }
}

int main() {
    resetFovArray(180);
    findAndSetPlayer();
    if (!checkWolrd()) {
        cout << "World is not valid" << endl;
        return 1;
    }

    // getVisibleBlockUP();
    // cout << "start rayCasting" << endl;
    // for (int l = 0; l < 10; l++) {
    //   rayCastDownRight(l);
    //   rayCastDownLeft(l);
    // }

    int i = 0;
    while (true) {
        rayCastInTheFov(10, i);
        system("clear");
        renderScreen();
        printScreen();
        cout << "angle " << i << endl;
        usleep(10000);
        i++;
        if(i == 360) i = 0;

    }

    return 0;
}

//░▒▓█