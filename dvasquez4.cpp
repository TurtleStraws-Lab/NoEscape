//author: Devin Vasquez
//
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "dvasquez4.h"

void Stickman() {
	glPushMatrix();
	GLfloat white[3] = {1.0f,1.0f,1.0f};
	glColor3fv(white);


	//Draw the head (a simple triangle)
	glBegin(GL_TRIANGLES);
	glVertex2f(-12.0f, -10.0f);  // Left part of head
	glVertex2f(0.0f, 20.0f);     // Top of head
	glVertex2f(12.0f, -10.0f);  // Right part of head
	glEnd();

	// Draw the body (a vertical line)
	glBegin(GL_LINES);
	glVertex2f(0.0f, -6.0f);  // Bottom of body
	glVertex2f(0.0f, 20.0f);  // Top of body
	glEnd();

	// Draw the arms (two slanted lines)
	glBegin(GL_LINES);
	glVertex2f(-12.0f, -6.0f);  // Left arm starting point
	glVertex2f(12.0f, -6.0f);   // Right arm starting point
	glEnd();

	// Draw the legs (two slanted lines)
	glBegin(GL_LINES);
	glVertex2f(0.0f, -6.0f);   // Left leg starting point
	glVertex2f(-10.0f, -20.0f); // Left leg end point
	glVertex2f(0.0f, -6.0f);   // Right leg starting point
	glVertex2f(10.0f, -20.0f); // Right leg end point
	glEnd();

	/*glPopMatrix();	
	  glPushMatrix();
	  glTranslatef(g.ship.pos[0], g.ship.pos[1],g.ship.pos[2]);
	  glBegin(GL_TRIANGLES);
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
	glColor3f(1.0,0.0f,0.0f);
	glBegin(GL_POINTS);
	glVertex2f(0.0f,0.0f);
	glEnd();
	glPopMatrix();	
	*/
}

