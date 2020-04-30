
/* Copyright (c) Mark J. Kilgard, 1994. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

   /* blender renders two spinning icosahedrons (red and green).
	  The blending factors for the two icosahedrons vary sinusoidally
	  and slightly out of phase.  blender also renders two lines of
	  text in a stroke font: one line antialiased, the other not.  */

#include "mpi.h"
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>
#define PERSPECTIVE_ON true

using namespace std;

int S = 0;

void drawField(void)
{
	//Football Field
	glColor4f(0, 1, 0, 1);
	glRectf(-25, -54, 25, 54);
	
	//White Field Lines
	glColor4f(1, 1, 1, 1);
	for (int i = 0; i <= 100; i += 10) {
		glRectf(-25, -51 + i, 25, -51 + i + 1);
	}
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawField();
	glutSwapBuffers();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

}

void idle(void)
{
	S += 0.01;
	glutPostRedisplay();
}

void visible(int vis)
{
	if (vis == GLUT_VISIBLE)
		glutIdleFunc(idle);
	else
		glutIdleFunc(NULL);
}

int main(int argc, char** argv)
{
	int mode = GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH;
	glutInit(&argc, argv);
	glutInitDisplayMode(mode);
	glutInitWindowSize(400, 400);
	glutCreateWindow("GaTech Buzzy Bowl");
	glutDisplayFunc(display);
	glutVisibilityFunc(visible);

	glNewList(1, GL_COMPILE);  /* create ico display list */
	glutSolidTetrahedron();
	glEndList();

	glMatrixMode(GL_PROJECTION);
	gluPerspective( /* field of view in degree */ 60.0,
		/* aspect ratio */ 1.0,
		/* Z near */ 1.0, /* Z far */ 200.0);
	glMatrixMode(GL_MODELVIEW);
	if (PERSPECTIVE_ON) {
		gluLookAt(0.0, -100.0, -50.0,  /* eye is at (0,0,5) */
			0.0, 0.0, 0.0,      /* center is at (0,0,0) */
			0.0, 1.0, 0.);      /* up is in positive Y direction */
	}
	else {
		gluLookAt(0.0, 0.0, 100.0,  /* eye is at (0,0,5) */
			0.0, 0.0, 0.0,      /* center is at (0,0,0) */
			0.0, 1.0, 0.);
	}
	
	glTranslatef(0.0, 0.0, -0.0);
	glutMainLoop();
	return 0;             /* ANSI C requires main to return int. */
}