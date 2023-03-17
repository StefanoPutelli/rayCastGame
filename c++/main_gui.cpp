#include <json/json.h>
#include <string.h>

#include <fstream>
#include <iostream>

#include "stuff/include/game.h"
#include "stuff/include/gui.h"

using namespace std;

float* fov_array;

static float playerX;
static float playerY;

static float direction;

static int FOV;
static int WIDTH;

static int RESOLUTION;
static int HEIGHT;

void setConf() {
    ifstream json_file;
    json_file.open("./stuff/conf/gui.json");
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
    direction = root["DIR"].asInt();
    RESOLUTION = WIDTH / FOV;
    fov_array = new float[WIDTH];
}

//TODO gestire meglio il world copy

int main(int argc, char* argv[]) {
    setConf();
    resetFovArray(WIDTH, fov_array);
    findAndSetPlayer(&playerX, &playerY);
    if (!checkWolrd()) {
        cout << "World is not valid" << endl;
        return 1;
    }
    while (true) {
        run_gui(fov_array, &playerX, &playerY, &direction, &FOV, &WIDTH, &RESOLUTION, &HEIGHT);
    }
}