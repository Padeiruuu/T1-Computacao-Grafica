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
class Ball {
private:
	Vec pos;
	double radius;
	Color color;
	Bezier* curve;
	Bezier* nextCurve;
	float progress;
	int progressDirection;
public: function<Bezier*(int)> onRequestCurve;

public:
	Ball(Vec pos, double radius, Color c, Bezier* curve)
		: pos(pos), radius(radius), color(c), curve(curve) {
			progressDirection = 1;
		}

	void update() {
		float prev = progress;
		progress += 0.01 * progressDirection;
		
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
		}

		// if we just crossed 50%
		if (prev < 0.5 && progress > 0.5 && onRequestCurve) {
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
