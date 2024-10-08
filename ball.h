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

#define BALL_FACES 3
#define BALL_SPEED 0.004

class Ball {
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

    Ball(Vec pos, double radius, Color c, Bezier* curve)
    : pos(pos), radius(radius), color(c), curve(curve), progress(0), angle(0) {
        progressDirection = 1;
        nextCurve = nullptr;
    }

    void update() {
        float prev = progress;
        progress += BALL_SPEED * progressDirection;

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
        glPushMatrix(); // Salvar o estado atual da matriz

        glTranslatef(pos.getX(), pos.getY(), 0); // Transladar para a posição do triângulo
        glRotatef(angle, 0, 0, 1); // Rotacionar ao redor do eixo Z

        glBegin(GL_POLYGON);
            color.glColor();
            for (int i = 0; i < BALL_FACES; i++) {
                Vec v = Vec(
                    cos(i / static_cast<float>(BALL_FACES) * M_PI * 2) * radius, 
                    sin(i / static_cast<float>(BALL_FACES) * M_PI * 2) * radius);
                v.glVertex();
            }
        glEnd(); 

        glPopMatrix(); // Restaurar o estado da matriz
    }
};

#endif // BALL_H
