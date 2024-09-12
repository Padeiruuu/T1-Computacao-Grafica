#include <GL/gl.h>
#include <iostream>
#include <stdio.h>
#include <math.h>

#include <sys/time.h>

#include <GL/glut.h>
#include <GL/glu.h>

#include "ball.h"
#include "bezier.h"
#include "color.h"


using namespace std;


void keyboard(int key, int x, int y) {
	switch (key) {
		case 'q': exit(0);
	}
}

Ball* ball = nullptr;
Bezier* curve = nullptr;

timeval last;
auto _ = gettimeofday(&last, NULL);
double accDt = 0;

void update() {
	timeval end_time;
	gettimeofday(&end_time, NULL);
	double dt = (float)(end_time.tv_sec - last.tv_sec) + 1.0e-6*(end_time.tv_usec - last.tv_usec);
	last = end_time;
	accDt += dt;

	if (accDt > 1.0/30) {
		accDt = 0;

		// update stuff

		ball->update();
		
		// end update
		
		glutPostRedisplay();
	}
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	curve->draw();
	ball->draw();

	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("basic window");

	curve = new Bezier(
		{0, 0},
		{0.2, 0.2},
		{-0.3, 0.2},
		{-0.3, -0.2},
		Color::Green
	);
	ball = new Ball({0, 0}, 0.03, Color::Blue, curve);

	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutSpecialFunc(keyboard);

	glClearColor(0.2, 0.2, 0.2, 0.2);

	glutMainLoop();

	cout << "hello, wolrd!" << endl;

	return 0;
}
