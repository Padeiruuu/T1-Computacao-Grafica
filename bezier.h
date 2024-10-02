#ifndef BEZIER_H
#define BEZIER_H


#include "color.h"
#include "vec.h"
#include <GL/gl.h>
#include <iostream>


float lerp(float p1, float p2, float t) {
	if (t < 0) return p1;	
	if (t > 1) return p2;	

	return t*(p2-p1) + p1;
}

Vec lerpVec(Vec v1, Vec v2, float t) {
	return Vec(lerp(v1.getX(), v2.getX(), t), lerp(v1.getY(), v2.getY(), t));
}

class Bezier {
private: 
	Vec p1, p2, p3, p4;
	Color color;

public:
	Bezier(Vec p1, Vec p2, Vec p3, Vec p4, Color c)
	: p1(p1), p2(p2), p3(p3), p4(p4), color(c) {
		std::cout << c << std::endl;
		std::cout << "aaaaaaaaaaaaaaaaaaaaaaa" << std::endl;
	}

	Vec at(float t) {
		Vec p5 = lerpVec(p1, p2, t);
		Vec p6 = lerpVec(p2, p3, t);
		Vec p7 = lerpVec(p3, p4, t);

		Vec p8 = lerpVec(p5, p6, t);
		Vec p9 = lerpVec(p6, p7, t);

		Vec final = lerpVec(p8, p9, t);
		return final;
	}

	void draw(const Color c) {
		c.glColor();

		glBegin(GL_LINE_STRIP);

		for (int i = 0; i < 32; i++) {
			float t = float(i) / 32;

			at(t).glVertex();
		}

		// make sure the line ends on the point
		p4.glVertex();

		glEnd();
	}

	void draw() {
		draw(this->color);
	}

	Vec getStartPoint() {
		return this->p1;
	}
	Vec getEndPoint() {
		return this->p4;
	}
};


#endif // BEZIER_H
