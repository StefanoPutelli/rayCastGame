#include <GL/glut.h>
#include <string.h>

// Dimensioni dell'immagine
const int IMAGE_WIDTH = 256;
const int IMAGE_HEIGHT = 256;

// Array di pixel di esempio
unsigned char screen[IMAGE_WIDTH][IMAGE_HEIGHT][4];

const int MAX_VIEW = 5;

int c = 0;

float* fov_array_gui;

void setColor(int x, int y, int r, int g, int b, int a) {
    screen[x][y][0] = r;
    screen[x][y][1] = g;
    screen[x][y][2] = b;
    screen[x][y][3] = a;
}

void renderDisplay() {
    for (int i = 0; i < IMAGE_WIDTH; i++) {
        if (fov_array_gui[i] == -1) {
            for (int l = 0; l < IMAGE_HEIGHT; l++) {
                setColor(i, l, 0, 0, 0, 255);
            }
        } else {
            int screenCenter = (int)(IMAGE_HEIGHT / 2);
            int halfHeight = (int)(screenCenter / fov_array_gui[i]);
            int start = screenCenter - halfHeight;
            int end = screenCenter + halfHeight;
            for (int l = 0; l < IMAGE_HEIGHT; l++) {
                if (l < start || l > end) {
                    setColor(i, l, 0, 0, 0, 255);
                } else {
                    if (fov_array_gui[i] > MAX_VIEW) {
                        setColor(i, l, 255, 255, 255, 255);
                    } else {
                        unsigned char col = (int)((float)(fov_array_gui[i] / MAX_VIEW) * 255);
                        setColor(i, l, col, col, col, 255);
                    }
                }
            }
        }
    }
}


void renderScreen(float* _fov_array) {
    // Inizializza glut e crea la finestra
    fov_array_gui = _fov_array;
    glClear(GL_COLOR_BUFFER_BIT);

    // Disegna l'immagine
    glDrawPixels(IMAGE_WIDTH, IMAGE_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screen);
    renderDisplay();
    // Richiede di ridisegnare la finestra
    glutSwapBuffers();
    glutPostRedisplay();

    // Avvia la visualizzazione

}

int main(int argc, char** argv) {
    char* myargv[1];
    int myargc = 1;
    myargv[0] = strdup("Myappname");
    glutInit(&myargc, myargv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowSize(IMAGE_WIDTH, IMAGE_HEIGHT);
    glutCreateWindow("Pixel Array");

    return 0;
}
