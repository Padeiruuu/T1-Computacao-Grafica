#ifndef BALL_H
#define BALL_H

#include <math.h>

#include <GL/gl.h>

#include "color.h"
#include "vec.h"
#include "bezier.h"

#define BALL_FACES 24
class Ball {
private:
	Vec pos;
	double radius;
	Color color;
	Bezier* curve;
	float progress;
	int progressDirection;

public:
	Ball(Vec pos, double radius, Color c, Bezier* curve)
		: pos(pos), radius(radius), color(c), curve(curve) {
			progressDirection = 1;
		}

	void update() {
		progress += 0.05 * progressDirection;
		if (progress > 1) {
			progressDirection = -1;
		}
		if (progress < 0) {
			progressDirection = 1;
		}
		pos = curve->at(progress);
	}

	void draw() {
		glBegin(GL_POLYGON);
			color.glColor();

			for (float i = 0; i < BALL_FACES; i++) {
				Vec v = pos + Vec(
						cos(i/BALL_FACES*M_PI*2)*radius, 
						sin(i/BALL_FACES*M_PI*2)*radius);
				v.glVertex();
			}
		glEnd(); 
	}
};

#endif // BALL_H
