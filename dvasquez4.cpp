//author: Devin Vasquez
//
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
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
	glBegin(GL_LINES);
    float angleRad = angle * M_PI / 180.0f;

    float endx = startx + length * cos(angleRad);
    float endy = starty + length * sin(angleRad);

    glVertex2f(startx,starty);
    glVertex2f(endx,endy);
    glEnd();
}
void DrawStickman::Stickman(float a, float b, float c ,float d) {
    
    //glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(a,b,c);
	glRotatef(d,0.0f,0.0f,1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    //draw the head
	drawCircle(1.0f,10.0f,10.0f,50);
    //draw the body
    calcAngle(1.25,-20,90,20);
    //draw left arm
    calcAngle(1.25,-2.5,225,12.5);
    //draw right arm
    calcAngle(1.25,-2.5,315,12.5);
    //draw left leg
    calcAngle(1.25,-20,235,12.5);
    //draw right leg
    calcAngle(1.25,-20,315,12.5);
    glEnd();
    glPopMatrix(); 
}

