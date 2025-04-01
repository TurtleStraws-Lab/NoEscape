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
}

void DrawStickman::Stickman(float a, float b, float c ,float d) {
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	glTranslatef(a,b,c);
	glRotatef(d,0.0f,0.0f,1.0f);
	drawCircle(0.0f,0.8f,0.1f,20);

	glBegin(GL_LINES);
	// Draw the body (a vertical line)
	glVertex2f(0.0f, 0.7f); 
	glVertex2f(0.0f, 0.4f);
	//left arm
	glVertex2f(0.0f, 0.6f);  
	glVertex2f(-0.2f, 0.5f);
	//right arm
	glVertex2f(0.0f, 0.6f);  
	glVertex2f(0.2f, 0.5f);
	//left leg
	glVertex2f(0.0f, 0.4f); 
	glVertex2f(-0.2f, 0.2f); 
	//right leg
	glVertex2f(0.0f, 0.4f); 
	glVertex2f(10.0f, 0.2f); 
	glEnd();

	glFlush();

}

