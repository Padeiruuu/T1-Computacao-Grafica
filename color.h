#ifndef COLOR_H
#define COLOR_H

#include <iostream>

class Color {
private:
    float r, g, b;

public:
    // Constructor
    Color(float r = 0.0f, float g = 0.0f, float b = 0.0f);

    // Set the OpenGL color
    void glColor() const;

		friend std::ostream& operator<<(std::ostream& os, const Color& c);

    // Preset colors
    static const Color Red;
    static const Color Green;
    static const Color DarkGreen;
    static const Color Blue;
    static const Color Cyan;
    static const Color Magenta;
    static const Color Yellow;
    static const Color White;
    static const Color Black;
};

#endif
