#include <GL/gl.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>

#include <sys/time.h>

#include <GL/glut.h>
#include <GL/glu.h>

// #include "ball.h"
#include "bezier.h"
#include "color.h"
#include "vec.h"


using namespace std;

vector<Vec*> *points = new vector<Vec*>;
Vec *draggin = nullptr;

void keyboard(int key, int x, int y) {
	switch (key) {
		case 'q': exit(0);
	}
}

// 0: mouse_left
// 1: mouse_rigth
// 2: mouse_middle
// 3: mouse_wheel_down
// 4: mouse_wheel_up
void mouse(int button, int up, int x, int y) {
	Vec* click = new Vec(float(x)/500*2 -1, -(float(y)/500*2-1));

	if (button == GLUT_LEFT_BUTTON && up && draggin == nullptr) {
		points->push_back(click);
	}	
	if (button == GLUT_LEFT_BUTTON && !up) {
		for (size_t i = 0; i < points->size(); i++) {
			if (points->at(i)->distance(*click) < 0.1) {
				draggin = points->at(i);
				printf("Draggin deez nuts\n");
			}
		}
	}
	if (button == GLUT_LEFT_BUTTON && up) {
		draggin = nullptr;
	}
}

void mouseMove(int x, int y) {
	printf("movin\n");
	if (draggin) {
		Vec click = {float(x)/500*2 -1, -(float(y)/500*2-1)};
		printf("x: %f, y: %f\n", click.getX(), click.getY());
		draggin->set(click);
	}
}

// Ball* ball = nullptr;
// Bezier* curve = nullptr;

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

		// ball->update();
		
		// end update
		
		glutPostRedisplay();
	}
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);

	// curve->draw();
	// ball->draw();
	
	glBegin(GL_POINTS);
	Color::Red.glColor();
	for (Vec* p : *points) {
		p->glVertex();	
	}
	glEnd();

	if (points->size() >= 4) {
		for (size_t i = 0; i < points->size()-3; i+=3) {
			Bezier *b = new Bezier(*points->at(i), *points->at(i+1), *points->at(i+2), *points->at(i+3), Color::Cyan);
			b->draw();
		}
	}
	
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("basic window");

	// curve = new Bezier(
	// 	{0, 0},
	// 	{0.2, 0.2},
	// 	{-0.3, 0.2},
	// 	{-0.3, -0.2},
	// 	Color::Green
	// );
	// ball = new Ball({0, 0}, 0.03, Color::Blue, curve);

	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutSpecialFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	

	glClearColor(0.2, 0.2, 0.2, 0.2);

	glutMainLoop();

	cout << "hello, wolrd!" << endl;

	return 0;
}
