#include "include/game.h"

using namespace std;

float getDecimals(float x) {
    return x - (int)x;
}

void resetFovArray(int WIDTH, float* fov_array) {
    for (int i = 0; i < WIDTH; i++) {
        fov_array[i] = -1.0;
    }
}

void resetMiniMap(unsigned char world_copy[Y][X]) {
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            world_copy[i][j] = world[i][j];
        }
    }
}

bool checkWolrd() {
    for (int i = 0; i < Y; i++) {
        if (world[i][0] != '#' || world[i][X - 1] != '#') {
            return false;
        }
    }
    for (int i = 0; i < X; i++) {
        if (world[0][i] != '#' || world[Y - 1][i] != '#') {
            return false;
        }
    }
    return true;
}

void saveInTheFov(int WIDTH, float* fov_array, int index, float dist) {
    fov_array[WIDTH - index] = abs(dist);
}

dirVars getDirVars(int angle, float playerX, float playerY) {
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

void markBlock(unsigned char world_copy[Y][X], int x, int y, unsigned char marker) {
    world_copy[y][x] = marker;
}

void rayCastInTheFov(int WIDTH, int FOV, int RESOLUTION, float* fov_array, unsigned char world_copy[Y][X], float direction, float playerX, float playerY) {
    float x_distance;
    float y_distance;
    float angle_rad;
    dirVars dirVal;
    float pre_angle;
    float angle;
    int dX;
    int dY;
    float sinAngle;
    float cosAngle;
    int sinSign;
    int cosSign;
    int dptX;
    int dptY;
    int start = (int)(direction - FOV / 2);
    for (int i = start; i < (int)(direction + FOV / 2); i++) {
        for (int r = 0; r < RESOLUTION; r++) {
            pre_angle = i % 360 + (float)r / RESOLUTION;
            angle = pre_angle < 0 ? pre_angle + 360 : pre_angle;
            dirVal = getDirVars(angle, playerX, playerY);
            angle_rad = (float)(angle * M_PI / 180);
            sinAngle = sin(angle_rad);
            cosAngle = cos(angle_rad);
            sinSign = sinAngle > 0 ? 1 : -1;
            cosSign = cosAngle > 0 ? 1 : -1;
            dptX = 0;
            dptY = 0;
            while (true) {
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
                int fov_index = (i - start) * RESOLUTION + r;
                float fish_eye_correction = cos((float)(fov_index / RESOLUTION - FOV / 2) * M_PI / 180);
                if (x_distance < y_distance) {
                    dX = (int)(playerX + (x_distance * cosAngle + tile_size / 2 * cosSign));
                    dY = (int)(playerY - (x_distance * sinAngle));
                    dptX++;
                    if (dY < 0 || dY >= Y || dX < 0 || dX >= X) {
                        saveInTheFov(WIDTH, fov_array, fov_index, -1);
                        break;
                    }
                    if (world[dY][dX] == '#') {
                        markBlock(world_copy, dX, dY, 'X');
                        cout<<fov_index<<endl;
                        saveInTheFov(WIDTH, fov_array, fov_index, abs(x_distance * fish_eye_correction));
                        break;
                    }
                } else {
                    dX = (int)(playerX + (y_distance * cosAngle));
                    dY = (int)(playerY - (y_distance * sinAngle + tile_size / 2 * sinSign));
                    dptY++;
                    if (dY < 0 || dY >= Y || dX < 0 || dX >= X) {
                        saveInTheFov(WIDTH, fov_array, fov_index, -1);
                        break;
                    }
                    if (world[dY][dX] == '#') {
                        markBlock(world_copy, dX, dY, 'Y');
                        cout<<fov_index<<endl;
                        saveInTheFov(WIDTH, fov_array, fov_index, abs(y_distance * fish_eye_correction));
                        break;
                    }
                }
            }
        }
    }
}

void findAndSetPlayer(float* playerX, float* playerY) {
    for (int i = 0; i < Y; i++) {
        for (int j = 0; j < X; j++) {
            if (world[i][j] == 'P') {
                *playerX = j + tile_size / 2;
                *playerY = i + tile_size / 2;
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

void captureKey(float* direction, float* playerX, float* playerY) {
    char key;
    float deltaY = 0;
    float deltaX = 0;
    key = getch();
    if (key == 'w') {
        deltaY = -0.1 * sin(*direction * M_PI / 180);
        deltaX = 0.1 * cos(*direction * M_PI / 180);
    }
    if (key == 's') {
        deltaY = 0.1 * sin(*direction * M_PI / 180);
        deltaX = -0.1 * cos(*direction * M_PI / 180);
    }
    if (key == 'a') {
        deltaY = -0.1 * cos(*direction * M_PI / 180);
        deltaX = -0.1 * sin(*direction * M_PI / 180);
    }
    if (key == 'd') {
        deltaY = 0.1 * cos(*direction * M_PI / 180);
        deltaX = 0.1 * sin(*direction * M_PI / 180);
    }
    if (key == 'k') {
        *direction = (*direction + VIEW_STEP) >= 360 ? *direction = 0 + *direction + VIEW_STEP - 360 : *direction + VIEW_STEP;
    }
    if (key == 'l') {
        *direction = (*direction - VIEW_STEP) < 0 ? *direction = 360 + *direction - VIEW_STEP : *direction - VIEW_STEP;
    }
    if (key == 'q') {
        exit(0);
    }
    if (deltaX != 0 || deltaY != 0) {
        if ((*playerX + deltaX < 0 || *playerX + deltaX >= X || *playerY + deltaY < 0 || *playerY + deltaY >= Y || world[(int)(*playerY + deltaY)][(int)(*playerX + deltaX)] != '#')) {
            cout << "playerX: " << *playerX << " playerY: " << *playerY << " deltaX: " << deltaX << " deltaY: " << deltaY << endl;
            *playerX += deltaX;
            *playerY += deltaY;
        }
    }
    cout << *playerX << " was " << *playerY << endl;
}

//░▒▓█