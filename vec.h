#ifndef VEC_H
#define VEC_H

class Vec {
private: 
	float x, y;

public:
	Vec(float x, float y) : x(x), y(y) {}

	float getX();
	float getY();

	void set(const Vec other);

	void glVertex(); 

	Vec rotated(const float angle) const;

	Vec operator+(const Vec& other) const; 

	Vec operator*(const float amount) const; 

	float distance(const Vec& other) const;

	void operator+=(const Vec& other); 

	bool operator==(const Vec& other) const;
	
	bool operator!=(const Vec& other) const;
};


#endif // VEC_H
