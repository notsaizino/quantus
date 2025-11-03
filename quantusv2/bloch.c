/**
 * bloch.c
 *
 * Implements drawing the Bloch sphere components using OpenGL.
 */
#include "bloch.h"
#include <GL/glut.h>
#include <math.h>
#include <complex.h>
#include <stdio.h>
#define M_PI 3.14159265358979323846


// Helper function to draw text on screen
void draw_text(float x, float y, float z, char *string) {
    glRasterPos3f(x, y, z);
    for (char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

// Helper function to draw text in 2D (orthographic)
void draw_text_2d(float x, float y, char *string) {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glRasterPos2f(x, y);
    for (char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void bloch_draw_sphere() {
    glColor4f(0.5f, 0.5f, 0.5f, 0.3f); 
    
    glPushMatrix();
    glutWireSphere(1.0, 16, 16);
    
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++) {
        float angle = 2.0f * M_PI * i / 100.0f;
        glVertex3f(cos(angle), sin(angle), 0.0f);
    }
    glEnd();
    glPopMatrix();
}

void bloch_draw_axes() {
    glBegin(GL_LINES);
    // Z-axis (Blue)
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(0, 0, -1.2f);
    glVertex3f(0, 0, 1.2f);
    
    // Y-axis (Green)
    glColor3f(0.3f, 1.0f, 0.3f);
    glVertex3f(0, -1.2f, 0);
    glVertex3f(0, 1.2f, 0);

    // X-axis (Red)
    glColor3f(1.0f, 0.3f, 0.3f);
    glVertex3f(-1.2f, 0, 0);
    glVertex3f(1.2f, 0, 0);
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
    draw_text(0.0f, 0.0f, 1.3f, "|0>");
    draw_text(0.0f, 0.0f, -1.4f, "|1>");
    draw_text(1.3f, 0.0f, 0.0f, "|+>");
    draw_text(-1.4f, 0.0f, 0.0f, "|->");
    draw_text(0.0f, 1.3f, 0.0f, "|+i>");
    draw_text(0.0f, -1.4f, 0.0f, "|-i>");
}

void bloch_draw_qubit(pqubit pq, float r, float g, float b) {
    float x = 0.0f, y = 0.0f, z = 0.0f;
    
    if (pq->measured != -1) {
        z = (pq->measured == 0) ? 1.0f : -1.0f;
    } else {
        // Use standard complex functions for GCC
        double complex alpha_conj = conj(pq->alpha);
        double complex product = alpha_conj * pq->beta;
        
        x = (float)(2.0 * creal(product));
        y = (float)(2.0 * cimag(product));
        z = (float)(creal(pq->alpha)*creal(pq->alpha) + cimag(pq->alpha)*cimag(pq->alpha) - 
                    creal(pq->beta)*creal(pq->beta) - cimag(pq->beta)*cimag(pq->beta));
    }
    
    glColor3f(r, g, b);
    glLineWidth(3.0f);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(x, y, z);
    glEnd();
    glLineWidth(1.0f);
}

void bloch_draw_text(double complex alpha, double complex beta, double omega_mhz, double brightness, int index) {
    char buffer[128];
    float x_shift = 0.05f + 0.4f * index;  // horizontal shift based on index
    glColor3f(1.0f, 1.0f, 1.0f);

    // Display alpha real and imaginary
    sprintf(buffer, "alpha: %.3f + %.3fi", creal(alpha), cimag(alpha));
    draw_text_2d(x_shift , 0.90f, buffer);

    // Display beta real and imaginary
    sprintf(buffer, "beta : %.3f + %.3fi", creal(beta), cimag(beta));
    draw_text_2d(x_shift, 0.85f, buffer);

    // Display brightness
    sprintf(buffer, "Brightness: %.3f", brightness);
    draw_text_2d(x_shift, 0.10f, buffer);

    // Display Rabi frequency
    sprintf(buffer, "Rabi Freq: %.2f MHz", omega_mhz);
    draw_text_2d(x_shift, 0.05f, buffer);

}