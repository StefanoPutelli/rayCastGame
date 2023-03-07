#include "game.h"

using namespace std;

int main() {
    resetFovArray();
    findAndSetPlayer();
    if (!checkWolrd()) {
        cout << "World is not valid" << endl;
        return 1;
    }
    while (true) {
        resetArrayCopy();
        captureKey();
        rayCastInTheFov(100);
        system("clear");
        printFovArray();
        renderScreen();
        printScreen();
        printMiniMap();
    }
    return 0;
}