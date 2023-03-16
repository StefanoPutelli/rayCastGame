#include <json/json.h>
#include <string.h>

#include <fstream>
#include <iostream>

#include "stuff/include/game.h"

using namespace std;

float* fov_array;

static float playerX;
static float playerY;

static float direction = 90;

unsigned char world_copy[Y][X];

static int FOV;
static int WIDTH;

static int RESOLUTION;
static int HEIGHT;

unsigned char** screen;

void setConf(int conf) {
    ifstream json_file;
    switch (conf) {
        case 0:
            json_file.open("./stuff/conf/terminal.json");
            break;
        case 1:
            json_file.open("./stuff/conf/gui.json");
            break;
        case 2:
            json_file.open("./stuff/conf/server.json");
            break;
        default:
            cerr << "conf value error" << endl;
            break;
    }
    if (!json_file.is_open()) {
        cerr << "Impossibile aprire il file di configurazione" << endl;
        exit(0);
    }
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(json_file, root)) {
        cerr << "Impossibile parsare il file di configurazione" << endl;
        exit(0);
    }
    FOV = root["FOV"].asInt();
    WIDTH = root["WIDTH"].asInt();
    HEIGHT = root["HEIGHT"].asInt();
    RESOLUTION = WIDTH / FOV;
    fov_array = new float[WIDTH];
    screen = new unsigned char*[WIDTH];
    for (int i = 0; i < WIDTH; i++) {
        screen[i] = new unsigned char[HEIGHT];
    }
}

void deleteMem() {
    for (int i = 0; i < WIDTH; i++) {
        delete[] screen[i];
    }
    delete[] screen;
}

//TODO deallocazione fatta bene

int main(int argc, char* argv[]) {
    if (argc == 1) {
        cout << "No arguments" << endl;
    } else {
        if (!strcmp(argv[1], "terminal")) {
            setConf(0);
            resetFovArray(WIDTH, fov_array);
            findAndSetPlayer(&playerX, &playerY);
            if (!checkWolrd()) {
                cout << "World is not valid" << endl;
                return 1;
            }
            while (true) {
                resetMiniMap(world_copy);
                captureKey(&direction, &playerX, &playerY);
                rayCastInTheFov(WIDTH, FOV, RESOLUTION, fov_array, world_copy, direction, playerX, playerY);
                renderScreen(fov_array, screen, WIDTH, HEIGHT);
                system("clear");
                printScreen(screen, direction, playerX, playerY, WIDTH, HEIGHT);
                printMiniMap(world_copy, playerX, playerY);
            }
        } else if (!strcmp(argv[1], "server")) {
            setConf(1);
            resetFovArray(WIDTH, fov_array);
            findAndSetPlayer(&playerX, &playerY);
            if (!checkWolrd()) {
                cout << "World is not valid" << endl;
                return 1;
            }
            while (true) {
                captureKey(&direction, &playerX, &playerY);
                rayCastInTheFov(WIDTH, FOV, RESOLUTION, fov_array, world_copy, direction, playerX, playerY);
            }
        } else if (!strcmp(argv[1], "gui")) {
            cout << "gui" << endl;
        } else {
            cout << "arg error" << endl;
        }
    }
}