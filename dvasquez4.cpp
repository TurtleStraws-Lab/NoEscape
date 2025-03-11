//author: Devin Vasquez
//
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
#include <string>


void Stickman() {
    glColor3fv(g.ship.color);
    glPushMatrix();
    glTranslatef(g.ship.pos[0], g.ship.pos[1], g.ship.pos[2]);
    glRotatef(g.ship.angle, 0.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    //Stickman();
    //glVertex2f(-12.0f, -10.0f);
    //glVertex2f(  0.0f, 20.0f);
    //glVertex2f( 0.0f, -6.0f);
    glVertex2f(  -12.0f, -10.0f);
    glVertex2f(  0.0f,  20.0f);
    glVertex2f(  0.0f,  -6.0f);
    glVertex2f(  0.0f,  -6.0f);
    glVertex2f(  0.0f,  20.0f);
    glVertex2f( 12.0f, -10.0f);
    glEnd();
   /* // head
    glVertex2f(  -0.05f,  0.08f);
    glVertex2f(  0.05f,  0.08f);
    glVertex2f( 0.0f, 0.09f);
    //body
    glVertex2f(  -0.02f, 0.04f);
    glVertex2f(  0.02f,  0.04f);
    glVertex2f(  0.0f,  0.08f);
    //legs
    glVertex2f(  0.05f,  0.0f);
    glVertex2f(  0.05f,  0.0f);
    glVertex2f( 0.0f, -0.04f);
*/
}

