//
//  curve.h
//  curvePractical
//
//  Created by Miles Crabill on 9/18/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_curve_h
#define curvePractical_curve_h

const int numberOfPoints = 101;

class Curve {
protected:
    // lookup table of points
    float2 points[numberOfPoints];
public:

    float rotationAngle = 0.0f;

    virtual float2 getPoint(float t) = 0;
    virtual float2 getDerivative(float t) = 0;

    std::vector<float2> isPointOnCurve(float2 p) {
        std::vector<float2> closePoints;
        for (int i = 0; i < numberOfPoints; i++) {
            if ((fabs(p.x - points[i].x) - 0.05f < 0.0f) && (fabs(p.y - points[i].y) - 0.05f < 0.0f)) {
                closePoints.push_back(points[i]);
            }
        }
        return closePoints;
    }

    void populatePoints() {
        for (int i = 0; i < numberOfPoints; i++) {
            points[i] = getPoint(i * (1.0 / (numberOfPoints - 1)));
        }
    }

    float2 lookupPoint(int i) {
        return points[i];
    }

    void draw() {
        glLoadIdentity();
        glPushMatrix();
        glRotatef(rotationAngle, 0, 0, 1);
        glBegin(GL_LINE_STRIP);
        // draw a line strip of small segments along the curve
        for (int i = 0; i < numberOfPoints; i++) {
            glVertex2f(points[i].x, points[i].y);
        }
        glEnd();
        glPopMatrix();
    }
    void drawTracker(float t) {
        // draw a point at: float2 r = getPoint(t)
        // draw a line between r and r + getDerivative(t)
        glLoadIdentity();
        glPushMatrix();
        glRotatef(rotationAngle, 0, 0, 1);

        float2 p = getPoint(t);
        glBegin(GL_POINTS);
        glVertex2f(p.x, p.y);
        glEnd();

        float2 d = getDerivative(t);
        glBegin(GL_LINES);
        glVertex2f(p.x, p.y);
        glVertex2f(p.x + d.x, p.y + d.y);
        glEnd();
        glPopMatrix();

    }

    void drawTangent(float t) {
        float2 p = getPoint(t);
        glBegin(GL_POINTS);
        glVertex2f(p.x, p.y);
        glEnd();

        float2 d = getDerivative(t);
        glBegin(GL_LINES);
        glVertex2f(p.x - d.x, p.y - d.y);
        glVertex2f(p.x + d.x, p.y + d.y);
        glEnd();
    }
};

#endif