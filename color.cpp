#include "color.h"

#include <GL/glut.h>

Color::Color(float r, float g, float b) : r(r), g(g), b(b) {}

void Color::glColor() const {
    glColor3f(r, g, b);
}

const Color Color::Red = Color(1.0f, 0.0f, 0.0f);
const Color Color::Green = Color(0.0f, 1.0f, 0.0f);
const Color Color::Blue = Color(0.0f, 0.0f, 1.0f);
const Color Color::Cyan = Color(0.0f, 1.0f, 1.0f);
const Color Color::Magenta = Color(1.0f, 0.0f, 1.0f);
const Color Color::Yellow = Color(1.0f, 1.0f, 0.0f);
const Color Color::White = Color(1.0f, 1.0f, 1.0f);
const Color Color::Black = Color(0.0f, 0.0f, 0.0f);
