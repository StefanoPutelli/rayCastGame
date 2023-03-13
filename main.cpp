#include "stuff/include/game.h"

using namespace std;

int main() {
    resetFovArray();
    findAndSetPlayer();
    if (!checkWolrd()) {
        cout << "World is not valid" << endl;
        return 1;
    }
    run();
    return 0;
}