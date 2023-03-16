#include <GL/glut.h>
#include <string.h>

#include <iostream>

#include "include/game.h"
#include "include/gui.h"

const int UPDATE_INTERVAL = 16;  // 60 fps

float* fov_array_ref;

float* playerX;
float* playerY;

float* direction;

unsigned char world_copy[Y][X];

int* FOV;
int* WIDTH;

int* RESOLUTION;
int* HEIGHT;

typedef struct Pixel {
    Pixel() {
        r = 0.0f;
        g = 0.0f;
        b = 0.0f;
    }
    Pixel(float r_, float g_, float b_) {
        r = r_;
        g = g_;
        b = b_;
    }
    float r, g, b;
} Pixel;

Pixel** screen_rgb;

void initVars(float* fov_array_, float* playerX_, float* playerY_, float* direction_, int* FOV_, int* WIDTH_, int* RESOLUTION_, int* HEIGHT_) {
    fov_array_ref = fov_array_;
    playerX = playerX_;
    playerY = playerY_;
    direction = direction_;
    FOV = FOV_;
    WIDTH = WIDTH_;
    RESOLUTION = RESOLUTION_;
    HEIGHT = HEIGHT_;
    screen_rgb = new Pixel*[*WIDTH];
    for (int i = 0; i < *WIDTH; i++) {
        screen_rgb[i] = new Pixel[*HEIGHT];
    }
}

void resetMapCopy() {
    for (int y = 0; y < Y; y++) {
        for (int x = 0; x < X; x++) {
            world_copy[y][x] = world[y][x];
        }
    }
}

void display() {
    // Cancella il buffer di colore
    glClear(GL_COLOR_BUFFER_BIT);
    for (int index = *WIDTH - 1; index >= 0; index--) {
        if (fov_array_ref[index] != -1) {
            int screenCenter = (int)(*HEIGHT / 2);
            int halfHeight = (int)(screenCenter / fov_array_ref[index]);
            int start = screenCenter - halfHeight;
            int end = screenCenter + halfHeight;
            glColor4f(0.0, 0.0, 1.0, 1.0f / fov_array_ref[index]);
            glBegin(GL_LINES);
            glVertex2f(index, start);
            glVertex2f(index, end);
            glEnd();
        }
    }

    // Visualizza il buffer di disegno
    glutSwapBuffers();
}

void update(int value) {
    // Modifica il contenuto dell'array screen[X][Y]
    //resetMapCopy();
    captureKey(direction, playerX, playerY);
    rayCastInTheFov(*WIDTH, *FOV, *RESOLUTION, fov_array_ref, world_copy, *direction, *playerX, *playerY);
    glutPostRedisplay();
    glutTimerFunc(UPDATE_INTERVAL, update, 0);
}

void reshape(int w, int h) {
    // Imposta la viewport per coprire l'intera finestra
    glViewport(0, 0, w, h);

    // Imposta la proiezione ortografica
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);

    // Imposta la matrice di modello-vista
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void run_gui(float* fov_array_, float* playerX_, float* playerY_, float* direction_, int* FOV_, int* WIDTH_, int* RESOLUTION_, int* HEIGHT_) {
    initVars(fov_array_, playerX_, playerY_, direction_, FOV_, WIDTH_, RESOLUTION_, HEIGHT_);
    char* myargv[1];
    int myargc = 1;
    myargv[0] = strdup("Myappname");
    glutInit(&myargc, myargv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    std::cout << *WIDTH << " " << *HEIGHT << std::endl;
    glutInitWindowSize(*WIDTH, *HEIGHT);
    glutCreateWindow("Pixel Array");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
}