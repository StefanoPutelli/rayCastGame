#include <termios.h>
#include <unistd.h>

#include <cmath>
#include <iostream>

using namespace std;

const int FOV = 60 + 1;
const int HEIGHT = 40;

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

const int X = 11;
const int Y = 10;

const int INT_MAX = 2147483647;

// up -> 0
// right -> 1
// down -> 2
// left -> 3

// 0 -> player not found
// 1 -> world not valid
// 2 -> step to large

// angle where the player is looking, 0 is left and 90 is up and 180 is right and 270 is down

const float tile_size = 1.0;

float playerX;
float playerY;

int step = 1;

int visibleBlock[10];

int direction = 0;

char screen[FOV - 1][HEIGHT];

float fov_array[FOV];

int world[Y][X] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

int world_copy[Y][X] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 2, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

float getDecimals(float x) {
    return x - (int)x;
}

void resetFovArray() {
    for (int i = 0; i < FOV; i++) {
        fov_array[i] = -1.0;
    }
}

void resetArrayCopy() {
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            world_copy[i][j] = world[i][j];
        }
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

void saveInTheFov(int index, float dist) {
    fov_array[index] = abs(dist);
}

// returns:
// >0 -> collision on x
// <0 -> collision on y
// 0 -> both, on the angle
// null -> no collision

dirVars getDirVars(int angle) {
    if (angle < 90) {
        if (angle == 0) {
            return dirVars(tile_size - getDecimals(playerX),
                           getDecimals(playerY));
        }
        return dirVars(tile_size - getDecimals(playerX),
                       getDecimals(playerY));
    } else if (angle < 180) {
        if (angle == 90) {
            return dirVars(getDecimals(playerX), getDecimals(playerY));
        }
        return dirVars(getDecimals(playerX), getDecimals(playerY));
    } else if (angle < 270) {
        if (angle == 180) {
            return dirVars(getDecimals(playerX),
                           tile_size - getDecimals(playerY));
        }
        return dirVars(getDecimals(playerX),
                       tile_size - getDecimals(playerY));
    } else {
        if (angle == 270) {
            return dirVars(tile_size - getDecimals(playerX),
                           tile_size - getDecimals(playerY));
        }
        return dirVars(tile_size - getDecimals(playerX),
                       tile_size - getDecimals(playerY));
    }
}

void markBlock(int x, int y, int marker) {
    world_copy[y][x] = marker;
}

void rayCastInTheFov(int depth, int direction) {
    float x_distance;
    float y_distance;
    float angle_rad;
    dirVars dirVal;
    int angle;
    int y_distance_perp;
    int x_distance_perp;
    int dX;
    int dY;
    for (int i = (int)(direction - FOV/2); i < (int)(direction + FOV/2); i++) {
        angle = (360 + i) % 360;
        cout << "angle: " << angle << endl;
        dirVal = getDirVars(angle);
        angle_rad = (float)angle * 3.14159 / 180;
        for (int dpt = 0; dpt < depth; dpt++) {
            if (angle == 0 || angle == 180) {
                y_distance = INT_MAX;
            } else {
                y_distance = (dirVal.PDistInnerBlockY + dpt * tile_size) / sin(angle_rad);
            }
            if (angle == 90 || angle == 270) {
                x_distance = INT_MAX;
            } else {
                x_distance = (dirVal.PDistInnerBlockX + dpt * tile_size) / cos(angle_rad);
            }
            if (x_distance < y_distance) {
                y_distance_perp = x_distance * sin(angle_rad);
                x_distance_perp = x_distance * cos(angle_rad);
                dX = (int)(playerX + x_distance_perp + 0.001);
                dY = (int)(playerY - y_distance_perp);
                if (dY < 0 || dY >= Y || dX < 0 || dX >= X) {
                    //cout << "out of bounds for angle: " << i << " angle: " << angle << endl;
                    continue;
                }
                if (world[dY][dX] == 1) {
                    markBlock(dX, dY, 3);
                    saveInTheFov(FOV - i, x_distance);
                    break;
                }
            } else {
                y_distance_perp = y_distance * sin(angle_rad);
                x_distance_perp = y_distance * cos(angle_rad);
                dX = (int)(playerX + x_distance_perp);
                dY = (int)(playerY - y_distance_perp + 0.001);
                if (dY < 0 || dY >= Y || dX < 0 || dX >= X) {
                    //cout << "out of bounds index: " << i << " angle: " << angle << endl;
                    continue;
                }
                if (world[dY][dX] == 1) {
                    markBlock(dX, dY, 3);
                    saveInTheFov(FOV - i, y_distance);
                    break;
                }
            }
        }
    }
}

void printFovArray() {
    for (int i = 0; i < FOV; i++) {
        if(fov_array[i] == -1)
            cout << "fov array error: " << i << endl;
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

void renderScreen() {
    for (int i = 0; i < FOV; i++) {
        if (fov_array[i] == -1) {
            for (int l = 0; l < HEIGHT; l++) {
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

void printScreen() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < FOV; j++) {
            cout << screen[j][i];
        }
        cout << endl;
    }
}

char getch() {
    char buf = 0;
    struct termios old = {
        0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}

void renderMiniMap() {
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            if (world_copy[i][j] == 1) {
                cout << "█";
            } else if (i == (int)playerY && j == (int)playerX) {
                cout << "P";
            } else if (world_copy[i][j] == 3) {
                cout << "▒";
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
}

void captureKey() {
    char key;
    key = getch();
    if (key == 'w') {
        playerY -= 0.1;
    }
    if (key == 's') {
        playerY += 0.1;
    }
    if (key == 'a') {
        playerX -= 0.1;
    }
    if (key == 'd') {
        playerX += 0.1;
    }
    if (key == 'k') {
        direction += 1;
    }
    if (key == 'l') {
        direction -= 1;
    }
    if (key == 'q') {
        exit(0);
    }
}

int main() {
    resetFovArray();
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

    while (true) {
        resetFovArray();
        resetArrayCopy();
        captureKey();
        if(direction < 0) direction = 360;
        rayCastInTheFov(100, direction);
        // system("clear");
        renderScreen();
        //printFovArray();
        printScreen();
        renderMiniMap();
        cout << "angle " << direction % 360 << endl;
        usleep(10000);
    }

    return 0;
}

//░▒▓█