#include <GL/freeglut_std.h>
#include <GL/gl.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
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

vector<Vec> points;
vector<Bezier*> curves;

vector<Bezier*> available;
Ball* ball = nullptr;

vector<Vec> pointsFromFile(const string& filename) {
	ifstream file(filename);

	if (!file) {
		cerr << "Error: Could not open the file " << filename << endl;
	}

	vector<Vec> points;

	string line;
	while (getline(file, line)) {
		istringstream iss(line);
		float x, y;
		if (!(iss >> x >> y)) {
			cerr << "Error: Malformed line: " << line << endl;
			continue;
		}

		points.push_back({x, y});
	}

	if (file.bad()) {
		cerr << "Error: Something went wrong while reading the file." << endl;
	}

	file.close();


	return points;
}

vector<Bezier*> curvesFromFile(const string& filename, vector<Vec> points) {
	ifstream file(filename);

	if (!file) {
		cerr << "Error: Could not open the file " << filename << endl;
	}

	vector<Bezier*> curves;

	string line;
	while (getline(file, line)) {
		istringstream iss(line);
		int p1, p2, p3, p4;

		if (!(iss >> p1 >> p2 >> p3 >> p4)) {
			cerr << "Error: Malformed line: " << line << endl;
			continue;
		}

		curves.push_back(new Bezier(
					points.at(p1), 
					points.at(p2), 
					points.at(p3), 
					points.at(p4), 
					Color::Black));
	}

	if (file.bad()) {
		cerr << "Error: Something went wrong while reading the file." << endl;
	}

	file.close();


	return curves;
}

template<typename T>
T pickRandomItem(const vector<T>& vec) {
    if (vec.empty()) {
        throw out_of_range("Vector is empty!");
    }

    static mt19937 rng(static_cast<unsigned>(time(0)));
    uniform_int_distribution<size_t> dist(0, vec.size() - 1);

    return vec.at(dist(rng));
}

vector<Bezier*> getCurvesAtPoint(const vector<Bezier*>& curves, Vec point) {
	vector<Bezier*> result;

	for (auto curve : curves) {
		if (curve->getStartPoint() == point || curve->getEndPoint() == point) {
			result.push_back(curve);
		}
	}

	return result;
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case 'q': exit(0); break;
		case 'a': {
			auto item = find(available.begin(), available.end(), ball->nextCurve);

			if (item == available.end()) {
				break;
			} 

			int index = distance(available.begin(), item);

			ball->nextCurve = available.at((index + 1) % available.size());

		} break;
		case 'd': {
			auto item = find(available.begin(), available.end(), ball->nextCurve);

			if (item == available.end()) {
				break;
			} 

			int index = distance(available.begin(), item);

			ball->nextCurve = available.at(index == 0 ? available.size()-1 : index-1);

		} break;
		case 'w': 
			ball->progressDirection = 1;
			ball->nextCurve = nullptr;
			available.clear();
			break;
		case 's':
			ball->progressDirection =-1;
			ball->nextCurve = nullptr;
			available.clear();
			break;
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
			c->draw(Color::DarkGreen);
		} else {
			c->draw();
		}
	}

	if (ball) {
		ball->draw();

		glBegin(GL_POINTS);

		Color::Yellow.glColor();

		ball->getEnd().glVertex();

		glEnd();

		ball->curve->draw(Color::Yellow);
		if (ball->nextCurve != nullptr) { ball->nextCurve->draw(Color::Green); }
	}
	
	
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("basic window");

	
	points = pointsFromFile("squircle_points.txt");
	if (points.size() == 0) {
		return 1;
	}
	curves = curvesFromFile("squircle_many_curves.txt", points);
	if (curves.size() == 0) {
		return 1;
	}


	ball = new Ball({0, 0}, 0.3, Color::DarkGreen, curves.at(0));
	

	ball->onRequestCurve = [](int direction) -> Bezier* {
		available = getCurvesAtPoint(curves, ball->getEnd());
		available.erase(remove_if( available.begin(), available.end(), 
					[](const Bezier* curr) { return curr == ball->curve; }));

		return available.at(0);
	};


	glutDisplayFunc(display);
	glutIdleFunc(update);
	glutKeyboardFunc(keyboard);
	// glutSpecialFunc(keyboard);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMove);
	

	glClearColor(0.4, 0.4, 0.4, 1);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-15, 15, -15, 15, -1.0, 1.0);

	// Switch back to the modelview matrix
	glMatrixMode(GL_MODELVIEW);

	glutMainLoop();

	cout << "hello, wolrd!" << endl;


	return 0;
}
