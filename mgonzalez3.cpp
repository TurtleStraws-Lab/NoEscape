//Moises Gonzalez
//2/11/25
//
//
//No Escape
//mgonzalez3.cpp

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "mgonzalez3.h"
#include <cmath>
#include <GL/glx.h>
using namespace std;
void mgonzalez3::darkBack(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);

    glClear(GL_COLOR_BUFFER_BIT);
}


void mgonzalez3::lighting(float pos1, float pos2, float area) {
    darkBack(0.0f, 0.0f, 0.0f, 1.0f); 
    glPushMatrix();
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    
    
    glColor4f(1.0f, 1.0f, 0.0f, 0.1f);

    glBegin(GL_TRIANGLE_FAN);

    glVertex2f(pos1, pos2);

    int circle = 100;

    for (int i = 0; i <= circle; i++) {
        float angle = (i / (float)circle) * 4.0f * M_PI;

        float x = pos1 + cos(angle) * area;
        float y = pos2 + sin(angle) * area;

        glVertex2f(x, y);
    }

    glEnd();

    glPopMatrix();
}

