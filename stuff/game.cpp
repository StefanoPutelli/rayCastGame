#include "include/game.h"

using namespace std;

float playerX;
float playerY;

int step = 1;

int visibleBlock[10];

float direction = 90;

float fov_array[WIDTH];

int world_copy[Y][X];

float getDecimals(float x) {
    return x - (int)x;
}

void resetFovArray() {
    for (int i = 0; i < WIDTH; i++) {
        fov_array[i] = -1.0;
    }
}

void resetMiniMap() {
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
    fov_array[WIDTH - index] = abs(dist);
}

dirVars getDirVars(int angle) {
    if (angle < 90) {
        return dirVars(tile_size - getDecimals(playerX),
                       getDecimals(playerY));
    } else if (angle < 180) {
        return dirVars(getDecimals(playerX), 
                       getDecimals(playerY));
    } else if (angle < 270) {
        return dirVars(getDecimals(playerX),
                       tile_size - getDecimals(playerY));
    } else {
        return dirVars(tile_size - getDecimals(playerX),
                       tile_size - getDecimals(playerY));
    }
}

void markBlock(int x, int y, int marker) {
    world_copy[y][x] = marker;
}

void rayCastInTheFov() {
    float x_distance;
    float y_distance;
    float angle_rad;
    dirVars dirVal;
    float angle;
    float y_distance_perp;
    float x_distance_perp;
    int dX;
    int dY;
    float sinAngle;
    float cosAngle;
    int sinSign;
    int cosSign;
    int dptX;
    int dptY;
    int start = (int)(direction - FOV/2);
    for (int i = start; i < (int)(direction + FOV/2); i++) {
        for(int r = 0; r < RESOLUTION; r++){
            angle = i % 360 + (float)r/RESOLUTION;
            dirVal = getDirVars(angle);
            angle_rad = (float)(angle * 3.14159 / 180);
            sinAngle = sin(angle_rad);
            cosAngle = cos(angle_rad);
            sinSign = sinAngle > 0 ? 1 : -1;
            cosSign = cosAngle > 0 ? 1 : -1;
            dptX = 0;
            dptY = 0;
            while(true) {
                if (angle == 0 || angle == 180) {
                    y_distance = INT_MAX;
                } else {
                    y_distance = ((dirVal.PDistInnerBlockY + (dptY * tile_size)) / abs(sinAngle));
                }
                if (angle == 90 || angle == 270) {
                    x_distance = INT_MAX;
                } else {
                    x_distance = ((dirVal.PDistInnerBlockX + (dptX * tile_size)) / abs(cosAngle));
                }
                int fov_index = (i - start)*RESOLUTION + r;
                if (x_distance < y_distance) {
                    y_distance_perp = x_distance * sinAngle;
                    x_distance_perp = x_distance * cosAngle + tile_size/2*cosSign;
                    dX = (int)(playerX + x_distance_perp);
                    dY = (int)(playerY - y_distance_perp);
                    dptX++;
                    if (dY < 0 || dY >= Y || dX < 0 || dX >= X) {
                        saveInTheFov(fov_index, -1);
                        break;
                    }
                    if (world[dY][dX] == 1) {
                        markBlock(dX, dY, 3);
                        cout<<fov_index<<endl;
                        saveInTheFov(fov_index, abs(x_distance * cos((float)(fov_index/RESOLUTION - FOV/2) * 3.14159 / 180)));
                        break;
                    }
                } else {
                    y_distance_perp = y_distance * sinAngle + tile_size/2*sinSign;
                    x_distance_perp = y_distance * cosAngle;
                    dX = (int)(playerX + x_distance_perp);
                    dY = (int)(playerY - y_distance_perp);
                    dptY++;
                    if (dY < 0 || dY >= Y || dX < 0 || dX >= X) {
                        saveInTheFov(fov_index, -1);
                        break;
                    }
                    if (world[dY][dX] == 1) {
                        markBlock(dX, dY, 4);
                        cout<<fov_index<<endl;
                        saveInTheFov(fov_index, abs(y_distance * cos((float)(fov_index/RESOLUTION - FOV/2)* 3.14159 / 180)));
                        break;
                    }
                }
            }
        }
    }
    
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



//TODO: adesso ce un fov array grande grande, bisogna in qualche modo processarlo per poretarlo a una risoluzione custom , cerando di fare antialiasing sui blocchi


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

void captureKey() {
    char key;
    float deltaY = 0;
    float deltaX = 0;
    key = getch();
    if (key == 'w') {
        deltaY = -0.1*sin(direction*3.14159/180);
        deltaX = 0.1*cos(direction*3.14159/180);
    }
    if (key == 's') {
        deltaY = 0.1*sin(direction*3.14159/180);
        deltaX = -0.1*cos(direction*3.14159/180);
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
    if((playerX + deltaX < 0 || playerX + deltaX >= X || playerY + deltaY < 0 || playerY + deltaY >= Y || world[(int)(playerY + deltaY)][(int)(playerX + deltaX)] != 1)){
        playerX += deltaX;
        playerY += deltaY;
    }
}

void run() {
    while (true) {
        resetMiniMap();
        captureKey();
        rayCastInTheFov();
        system("clear");
        renderScreen(fov_array);
        printScreen();
        printMiniMap(world_copy, playerX, playerY);
    }
}



//░▒▓█