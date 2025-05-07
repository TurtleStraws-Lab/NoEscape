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

bool isWalking = false;
float thighAngleLeft = 240.0f;
float shinAngleLeft = 240.0f;
float thighAngleRight = 300.0f;
float shinAngleRight = 300.0f;
int walkFrame = 0;
int walkingDirection = 0;
bool stepToggle = false;

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
calcAngle::calcAngle(float startx,float starty, float angle, float length) {
    glBegin(GL_LINE_LOOP);
    float angleRad = angle * M_PI / 180.0f;

    float endx = startx + length * cos(angleRad);
    float endy = starty + length * sin(angleRad);

    glVertex2f(startx,starty);
    glVertex2f(endx,endy);
    glEnd();
}
void DrawShip::Ship(float a, float b, float c,float d) {
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
    drawCircle(0.0f,10.0f,2.0f,5);
    //draw the body
    calcAngle(0.0f,0.0f,90,10);
    //draw left arm
    calcAngle(0,5,225,5);
    //draw right arm
    calcAngle(0,5,315,5);
    //draw left leg
    //
    //
    float thighLen = 4.0f;
    float shinLen = 4.0f;

    float hipLX = -0.5f;
    float hipLY = 0.0f;
    float kneeLX = hipLX + thighLen * cos(thighAngleLeft * M_PI / 180.0f);
    float kneeLY = hipLY + thighLen * sin(thighAngleLeft * M_PI / 180.0f);
    calcAngle(hipLX, hipLY, thighAngleLeft, thighLen);
    calcAngle(kneeLX, kneeLY, shinAngleLeft, shinLen);

    // RIGHT leg
    float hipRX = -0.5f;
    float hipRY = 0.0f;
    float kneeRX = hipRX + thighLen * cos(thighAngleRight * M_PI / 180.0f);
    float kneeRY = hipRY + thighLen * sin(thighAngleRight * M_PI / 180.0f);
    calcAngle(hipRX, hipRY, thighAngleRight, thighLen);
    calcAngle(kneeRX, kneeRY, shinAngleRight, shinLen);
    glEnd();
    glPopMatrix(); 
}
void resetStickman() {
    isWalking = false;
    stepToggle = false;
    thighAngleLeft = 240.0f;
    shinAngleLeft = 240.0f;
    thighAngleRight = 300.0f;
    shinAngleRight = 300.0f;
}
void Movement(Display *display, XEvent &event) {
    if (event.type == KeyPress) {
        KeySym key = XkbKeycodeToKeysym(display,event.xkey.keycode,0,0);
        if (key == XK_Left) {
            updateShipAngle(270.0f);
        }
        if (key == XK_Right) {
            updateShipAngle(90.0f);
        }
        if (key == XK_Up) {
            updateShipAngle(180.0f);
        }
        if (key == XK_Down) {
            updateShipAngle(0.0f);
        }
    }
}
void updateWalkingLegs() {
    if (!isWalking) return;

    if (walkFrame == 0) {
        thighAngleLeft = 235.0f;  // lifted back
        shinAngleLeft  = 235.0f;  // straight down
        thighAngleRight = 295.0f; // forward
        shinAngleRight  = 295.0f; // slight bend
        walkFrame = 1;
    } else {
        thighAngleLeft = 290;
        shinAngleLeft  = 310;
        thighAngleRight = 250;
        shinAngleRight  = 270;
        walkFrame = 0;
    }

    isWalking = false;
} 
void updateLegAngle(int v,bool isWalking) {
    if (isWalking == false) {
        // Reset to default when not walking
        thighAngleLeft = 235.0f;
        shinAngleLeft  = 235.0f;
        thighAngleRight = 310.0f;
        shinAngleRight  = 310.0f;
        return;
    }
    if (v == 0 && isWalking == true) {
        if (stepToggle) {
            // One leg forward, one back
            thighAngleLeft = 245.0f;
            shinAngleLeft  = 245.0f;
            thighAngleRight = 300.0f;
            shinAngleRight  = 300.0f;
        } else {
            // Reverse the legs
            thighAngleLeft = 290.0f;
            shinAngleLeft  = 310.0f;
            thighAngleRight = 250.0f;
            shinAngleRight  = 270.0f;
        }
    } 
    if (v == 1 && isWalking == true) {
        if (stepToggle) {
            // One leg forward, one back
            thighAngleLeft = 280.0f;
            shinAngleLeft  = 200.0f;
            thighAngleRight = 290.0f;
            shinAngleRight  = 290.0f;
        } else {
            // Reverse the legs
            thighAngleLeft = 250.0f;
            shinAngleLeft  = 250.0f;
            thighAngleRight = 275.0f;
            shinAngleRight  = 295.0f;
        }
    }

    stepToggle = !stepToggle;
}
void MovementWalking(Display *display, XEvent &event) {
    if (event.type == KeyPress) {
        KeySym key = XkbKeycodeToKeysym(display,event.xkey.keycode,0,0);
        if (key == XK_Left) {
            bool isWalking = true;
            int left = 0;
            updateLegAngle(left,isWalking);
        }
        if (key == XK_Right) {
            bool isWalking = true;
            int right = 1;
            updateLegAngle(right,isWalking);
        }

        if (event.type == KeyRelease) {
            //KeySym key = XkbKeycodeToKeysym(display,event.xkey.keycode,0,0);
            //if (key == XK_Left || key == XK_Right){
            isWalking = false;
        }
        }
    }

