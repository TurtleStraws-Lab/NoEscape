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
// Function to set the background color to a dark color with transparency
void mgonzalez3::darkBack(float r, float g, float b, float a) {
    // Set the clear color to the given RGBA values
    glClearColor(r, g, b, a);

    // Clear the color buffer (the screen), using the previously set color
    glClear(GL_COLOR_BUFFER_BIT);
}

// Function to blend the lighting effect
void glBlendFunc(GLenum sfactor, GLenum dfactor);

// Function to create a circular lighting effect at a specified position
void mgonzalez3::lighting(float pos1, float pos2, float area) {
    // Save the current matrix state
    glPushMatrix();
    // Enable blending for transparent or semi-transparent effects
    glEnable(GL_BLEND);

    // Set the color for the lighting (yellow, with some alpha value)
    glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

    // Start drawing a filled circle (triangle fan)
    glBegin(GL_TRIANGLE_FAN);

    // Set the center of the circle at the position (pos1, pos2)
    glVertex2f(pos1, pos2);

    // Set the number of vertices for the circle (higher values result in a smoother circle)
    int circle = 100;

    // Loop through each vertex of the circle
    for (int i = 0; i <= circle; i++) {
        // Calculate the angle for each vertex in radians (full circle = 2π)
        float angle = (i / (float)circle) * 4.0f * M_PI;

        // Calculate the x and y position of each vertex based on the angle
        float x = pos1 + cos(angle) * area;
        float y = pos2 + sin(angle) * area;

        // Draw the vertex at the calculated position
        glVertex2f(x, y);
    }

    // End drawing the circle
    glEnd();

    // Restore the previous matrix state
    glPopMatrix();
}

