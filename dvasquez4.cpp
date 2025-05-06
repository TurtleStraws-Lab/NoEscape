//author: Devin Vasquez
//
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#include "dvasquez4.h"


drawCircle::drawCircle(float x2,float y2,float r,int num_seg) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < num_seg;i++) {
        float theta = 2.0f * 3.1415926f * float(i) / float(num_seg);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(x + x2, y + y2);
    }
    glEnd();
}
calcAngle::calcAngle(float startx,float starty, float angle, float length) 
{
    glBegin(GL_LINE_LOOP);
    float angleRad = angle * M_PI / 180.0f;

    float endx = startx + length * cos(angleRad);
    float endy = starty + length * sin(angleRad);

    glVertex2f(startx,starty);
    glVertex2f(endx,endy);
    glEnd();
}
void DrawShip::Ship(float a, float b, float c,float d){
    glPushMatrix();
    glTranslatef(a,b,c);
    glRotatef(d,0.0f,0.0f,1.0f);
    glColor3f(1.0,1.0f,1.0f);
    calcAngle(0,0,75,11);
    calcAngle(0,0,105,11);
    calcAngle(0,5,115,6);
    calcAngle(0,5,65,6);
    glEnd();
    glPopMatrix();


}
void DrawStickman::Stickman(float a, float b, float c,float d) {
    
    glPushMatrix();
    glTranslatef(a,b,c);
    glRotatef(d,0.0f,0.0f,1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    //draw the head
    drawCircle(1.0f,10.0f,2.0f,5);
    //draw the body
    calcAngle(1.15,-1.15,90,10);
    //draw left arm
    calcAngle(1,5,225,5);
    //draw right arm
    calcAngle(1,5,315,5);
    //draw left leg
    calcAngle(0.25,0,235,6.5);
    //draw right leg
    calcAngle(1,0,310,6.5);
    glEnd();
    glPopMatrix(); 
}
void Movement(Display *display, XEvent &event) {
    if (event.type == KeyPress) {
        KeySym key = XkbKeycodeToKeysym(display,event.xkey.keycode,0,0);
        if(key == XK_Left) {
            updateShipAngle(270.0f);
        }
        if(key == XK_Right) {
            updateShipAngle(90.0f);
        }
        if(key == XK_Up) {
            updateShipAngle(180.0f);
        }
        if(key == XK_Down) {
            updateShipAngle(0.0f);
        }
    }
}
