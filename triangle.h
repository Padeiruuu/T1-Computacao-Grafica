#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <stdio.h>
#include <math.h>
#include <functional>

#include <GL/gl.h>

#include "color.h"
#include "vec.h"
#include "bezier.h"

using namespace std;

#define TRIANGLE_FACES 3
#define TRIANGLE_SPEED 0.004

class Triangle {
public: 
    Vec pos;
    double radius;
    Color color;
    Bezier* curve;
    Bezier* nextCurve;
    float progress;
    int progressDirection;
    float angle;
    function<Bezier*(int)> onRequestCurve;

    Triangle(Vec pos, double radius, Color c, Bezier* curve)
    : pos(pos), radius(radius), color(c), curve(curve), progress(0), angle(0) {
        progressDirection = 1;
        nextCurve = nullptr;
    }

    void update() {
        float prev = progress;
        progress += TRIANGLE_SPEED * progressDirection;

        if (progress > 1 || progress < 0) {
            // we end on the next curve start
            if (nextCurve && getEnd() == nextCurve->getStartPoint()) { 
                progress = 0;
                progressDirection = 1;
                curve = nextCurve;
                nextCurve = nullptr;
            } else {
                progress = (progress > 1) ? 1 : 0;
                progressDirection = (progressDirection == 1) ? -1 : 1;
            }
        }
        // if ball is moving forward, is halfway and doesn't have a next curve
		// or is moving backwards
        if ((progressDirection >= 1 && progress > 0.5 && nextCurve == nullptr) ||
            (progressDirection <= -1 && progress < 0.5 && nextCurve == nullptr)) {
            nextCurve = onRequestCurve(progressDirection);
        }

        if (curve) {
            pos = curve->at(progress);

            float newProgress = progress + (progressDirection * 0.01f);
            newProgress = std::max(0.0f, std::min(newProgress, 1.0f));

            float deltaX = curve->at(newProgress).getX() - pos.getX();
            float deltaY = curve->at(newProgress).getY() - pos.getY();

            angle = atan2(deltaY, deltaX) * (180.0 / M_PI);
        }
    }

    float getX() { return pos.getX(); }
    float getY() { return pos.getY(); }

    Vec getEnd() { 
        return progressDirection > 0 ? curve->getEndPoint() : curve->getStartPoint();
    }

    void draw() {
        glPushMatrix();

        glTranslatef(pos.getX(), pos.getY(), 0);
        glRotatef(angle, 0, 0, 1);

        glBegin(GL_TRIANGLES);
            color.glColor();
            for (int i = 0; i < TRIANGLE_FACES; i++) {
                Vec v = Vec(
                    cos(i / static_cast<float>(TRIANGLE_FACES) * M_PI * 2) * radius, 
                    sin(i / static_cast<float>(TRIANGLE_FACES) * M_PI * 2) * radius);
                v.glVertex();
            }
        glEnd(); 

        glPopMatrix();
    }
};

#endif // TRIANGLE_H
