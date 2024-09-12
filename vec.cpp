#include <GL/glut.h>
#include <math.h>

#include "vec.h"

void Vec::glVertex() {
	glVertex2f(this->x, this->y);
}

float Vec::getX() {
	return x;
}
float Vec::getY() {
	return y;
}

Vec Vec::rotated(const float angle) const {
	return Vec(
			x*cos(angle) - y*sin(angle), 
			x*sin(angle) + y*cos(angle));
}

Vec Vec::operator+(const Vec& other) const {
	return Vec(x + other.x, y + other.y);
}

Vec Vec::operator*(const float amount) const {
	return Vec(x*amount, y*amount);
}

void Vec::operator+=(const Vec& other) {
	x += other.x;
	y += other.y;
}
