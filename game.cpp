#include "game.h"

using namespace std;

float playerX;
float playerY;

int step = 1;

int visibleBlock[10];

float direction = 0;

int screen[FOV][HEIGHT];

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
    fov_array[FOV - index] = abs(dist);
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

void rayCastInTheFov(int depth) {
    float x_distance;
    float y_distance;
    float angle_rad;
    dirVars dirVal;
    int angle;
    float y_distance_perp;
    float x_distance_perp;
    int dX;
    int dY;
    float sinAngle;
    float cosAngle;
    int start = (int)(direction - FOV/2);
    for (int i = start; i < (int)(direction + FOV/2); i++) {
        angle = (360 + i) % 360;
        dirVal = getDirVars(angle);
        angle_rad = (float)angle * 3.14159 / 180;
        sinAngle = sin(angle_rad);
        cosAngle = cos(angle_rad);
        for (int dpt = 0; dpt < depth; dpt++) {
            if (angle == 0 || angle == 180) {
                y_distance = INT_MAX;
            } else {
                y_distance = ((dirVal.PDistInnerBlockY + dpt) * tile_size * abs(sinAngle));
            }
            if (angle == 90 || angle == 270) {
                x_distance = INT_MAX;
            } else {
                x_distance = ((dirVal.PDistInnerBlockX + dpt) * tile_size * abs(cosAngle));
            }
            if (x_distance < y_distance) {
                y_distance_perp = -x_distance * sinAngle;
                x_distance_perp = x_distance * cosAngle;
                dX = (int)(playerX + x_distance_perp);
                dY = (int)(playerY + y_distance_perp);
                if (dY < 0 || dY >= Y || dX < 0 || dX >= X) {
                    saveInTheFov(i - start - 1, -1);
                    break;
                }
                if (world[dY][dX] == 1) {
                    markBlock(dX, dY, 3);
                    saveInTheFov(i - start - 1, x_distance * cos(abs((direction - i) * 3.14159 / 180)));
                    break;
                }
            } else {
                y_distance_perp = -y_distance * sinAngle;
                x_distance_perp = y_distance * cosAngle;
                dX = (int)(playerX + x_distance_perp);
                dY = (int)(playerY + y_distance_perp);
                if (dY < 0 || dY >= Y || dX < 0 || dX >= X) {
                    saveInTheFov(i - start - 1, -1);
                    break;
                }
                if (world[dY][dX] == 1) {
                    markBlock(dX, dY, 3);
                    saveInTheFov(i - start - 1, y_distance * cos(abs((direction - i) * 3.14159 / 180)));
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
        for (int j = 0; j < FOV; j++) {
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

void printMiniMap() {
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
        playerY -= 0.1*sin(direction*3.14159/180);
        playerX += 0.1*cos(direction*3.14159/180);
    }
    if (key == 's') {
        playerY += 0.1*sin(direction*3.14159/180);
        playerX -= 0.1*cos(direction*3.14159/180);
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
    if(direction < 0) direction = 360;
}



//░▒▓█