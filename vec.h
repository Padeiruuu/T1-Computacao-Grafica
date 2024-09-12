#ifndef VEC_H
#define VEC_H

class Vec {
private: 
	float x, y;

public:
	Vec(float x, float y) : x(x), y(y) {}

	float getX();
	float getY();

	void glVertex(); 

	Vec rotated(const float angle) const;

	Vec operator+(const Vec& other) const; 

	Vec operator*(const float amount) const; 

	void operator+=(const Vec& other); 
};


#endif // VEC_H
