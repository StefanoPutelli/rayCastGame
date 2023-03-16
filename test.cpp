#include <GL/glut.h>
#include <math.h>

const int UPDATE_INTERVAL = 16;  // 60 fps

float alpha = 1.0f; // variabile di opacità

void display() {
    // Cancella il buffer di colore
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Disegna un cerchio
    glColor4f(1.0f, 0.0f, 0.0f, alpha); // rosso con opacità alpha
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f); // centro del cerchio
    for (int i = 0; i <= 360; i += 10) {
        float angle = i * 3.14159265358979323846f / 180.0f;
        float x = cos(angle);
        float y = sin(angle);
        glVertex2f(x, y);
    }
    glEnd();

    // Visualizza il buffer di disegno
    glutSwapBuffers();
}

void update(int value) {
    // Modifica il contenuto dell'array screen[X][Y]
    alpha -= 0.01f; // diminuisce l'opacità di 0.01 ogni volta che l'aggiornamento viene chiamato
    if (alpha < 0.0f) alpha = 1.0f; // se l'opacità diventa negativa, la reimposta a 1.0
    glutPostRedisplay();
    glutTimerFunc(UPDATE_INTERVAL, update, 0);
}

void reshape(int w, int h) {
    // Imposta la viewport per coprire l'intera finestra
    glViewport(0, 0, w, h);

    // Imposta la proiezione ortografica
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1); // imposta un sistema di coordinate in cui il lato del quadrato OpenGL va da -1 a 1

    // Imposta la matrice di modello-vista
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(400, 400);
    glutCreateWindow("Circle Opacity");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, update, 0);
    glutMainLoop();
    return 0;
}
