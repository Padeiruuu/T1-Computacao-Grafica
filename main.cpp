#include <GL/gl.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <math.h>
#include <random>
#include <ctime>

#include <sys/time.h>

#include <GL/glut.h>
#include <GL/glu.h>

#include "ball.h"
#include "bezier.h"
#include "color.h"
#include "vec.h"


using namespace std;

vector<Vec> points = { {-0.9,0.9}, {0.9,0.9}, {0.9,-0.9}, {-0.9,-0.9} };
vector<Bezier*> curves;
vector<Bezier*> available;
Ball* ball = nullptr;

template<typename T>
T pickRandomItem(const std::vector<T>& vec) {
    if (vec.empty()) {
        throw out_of_range("Vector is empty!");
    }

    // Seed the random generator
    mt19937 rng(static_cast<unsigned>(time(0)));  // Or use std::random_device
    // Create a uniform distribution for valid indices
    uniform_int_distribution<size_t> dist(0, vec.size() - 1);

    // Pick a random index and return the corresponding item
    return vec.at(dist(rng));
}

// Assuming Vec is your point/vector class, and Bezier has getStartPoint() and getEndPoint()
std::vector<Bezier*> getCurvesAtPoint(const std::vector<Bezier*>& curves, Vec point) {
	std::vector<Bezier*> result;

	for (auto curve : curves) {
		if (curve->getStartPoint() == point || curve->getEndPoint() == point) {
			result.push_back(curve);
		}
	}

	return result;
}

void keyboard(int key, int x, int y) {
	switch (key) {
		case 'q': exit(0);
	}
}

void mouse(int button, int up, int x, int y) {
	/*
	Vec click = new Vec(float(x)/500*2 -1, -(float(y)/500*2-1));

	if (button == GLUT_LEFT_BUTTON && up) {
		if (points.size() == 4) {
			auto last = points.at(3);
			auto b = new Bezier(points.at(0), points.at(1), points.at(2), last, Color::Red);
			curves.push_back(b);
			points.clear();
			points.push_back(last);

			if (!ball) {
				ball = new Ball({0,0}, 0.1, Color::Magenta, curves.at(0));
			}
		}
	}	
	*/
}

void mouseMove(int x, int y) {

}


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

		if (ball) ball->update();
		
		// end update
		
		glutPostRedisplay();
	}
}


void display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(4);

	glBegin(GL_POINTS);
	Color::Blue.glColor();
	for (Vec p : points) {
		p.glVertex();	
	}
	glEnd();

	for (Bezier* c : curves) {
    auto it = find(available.begin(), available.end(), c);

		if (it != available.end()) {
			// is available
			c->draw(Color::Yellow);
		} else {
			c->draw();
		}
	}

	if (ball) {
		ball->draw();

		glBegin(GL_POINTS);

		Color::Red.glColor();

		ball->getEnd().glVertex();

		glEnd();
	}
	
	
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutCreateWindow("basic window");

	curves = {
		// new Bezier(points.at(0), points.at(2), points.at(3), points.at(1), Color::Blue),
		new Bezier(points.at(0), points.at(1), points.at(2), points.at(3), Color::Blue),

		new Bezier(points.at(1), points.at(2), points.at(3), points.at(0), Color::Blue),
		// new Bezier(points.at(1), points.at(3), points.at(0), points.at(2), Color::Blue),

		new Bezier(points.at(2), points.at(3), points.at(0), points.at(1), Color::Blue),
		// new Bezier(points.at(2), points.at(0), points.at(1), points.at(3), Color::Blue),

		// new Bezier(points.at(3), points.at(1), points.at(2), points.at(0), Color::Blue),
		new Bezier(points.at(3), points.at(0), points.at(1), points.at(2), Color::Blue),
	};

	ball = new Ball({0, 0}, 0.03, Color::Green, curves.at(0));
	

	ball->onRequestCurve = [](int direction) -> Bezier* {
		auto avaliable = getCurvesAtPoint(curves, ball->getEnd());
		auto curve = pickRandomItem(avaliable);
	
		return curve;
	};

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
