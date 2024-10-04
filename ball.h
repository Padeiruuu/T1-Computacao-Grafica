#ifndef BALL_H
#define BALL_H

#include <stdio.h>
#include <math.h>
#include <functional>

#include <GL/gl.h>

#include "color.h"
#include "vec.h"
#include "bezier.h"

using namespace std;

#define BALL_FACES 24
#define BALL_SPEED 0.01
class Ball {
public: 
	Vec pos;
	double radius;
	Color color;
	Bezier* curve;
	Bezier* nextCurve;
	float progress;
	int progressDirection;
	function<Bezier*(int)> onRequestCurve;

	Ball(Vec pos, double radius, Color c, Bezier* curve)
		: pos(pos), radius(radius), color(c), curve(curve) {
			progressDirection = 1;
			nextCurve = nullptr;
		}

	void update() {
		float prev = progress;
		progress += BALL_SPEED * progressDirection;
		
		if (progress > 1 || progress < 0) {
			// we end on the next curve start
			if (getEnd() == nextCurve->getStartPoint()) { 
				progress = 0;
				progressDirection = 1;
			} else {
				progress = 1;
				progressDirection = -1;
			}
			curve = nextCurve;
			nextCurve = nullptr;
		}

		// if ball is moving forward, is halfway and doesn't have a next curve
		// or is moving backwards
		if ((progressDirection >= 1 && progress > 0.5 && nextCurve == nullptr)
			||(progressDirection <=-1 && progress < 0.5 && nextCurve == nullptr)) {
			nextCurve = onRequestCurve(progressDirection);
		}

		pos = curve->at(progress);
	}

	float getX() { return pos.getX(); }
	float getY() { return pos.getY(); }

	Vec getEnd() { 
		return progressDirection > 0 ? curve->getEndPoint() : curve->getStartPoint();
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
