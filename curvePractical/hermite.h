//
//  hermite.h
//  curvePractical
//
//  Created by Miles Crabill on 10/17/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_hermite_h
#define curvePractical_hermite_h

#include "freeformcurve.h"
#include "float2.h"
#include <vector>

class Hermite : public Freeform {
protected:
    std::vector<float2> tangentPoints;
    std::vector<float2> combinedPoints;
public:

    int curveType() {
        return 3;
    }

    // overriding because have separate array of tangentpoints
    void addControlPoint(float2 p) {
        controlPoints.push_back(p);
        tangentPoints.push_back(p - float2(0.2f, 0.2f));
    }

    // combines tangentPoints and controlPoints into one vector
    // so i don't have to duplicate a ton of code
    void combinePoints() {
        combinedPoints.clear();
        combinedPoints.insert(combinedPoints.end(), controlPoints.begin(), controlPoints.end());
        combinedPoints.insert(combinedPoints.end(), tangentPoints.begin(), tangentPoints.end());
    }

    // overriding because have separate array of tangentpoints
    // based on index we know which array we're accessing from
    // because of how getClosestControlPoints returns indices
    float2 getControlPoint(int index) {
        if (index >= controlPoints.size()) {
            combinePoints();
            return combinedPoints[index];
        } else {
            return controlPoints[index];
        }
    }

    // overriding because have separate array of tangentpoints
    void setControlPoint(int index, float2 p) {
        if (index >= controlPoints.size()) {
            tangentPoints[index - controlPoints.size()] = p;
        } else {
            controlPoints[index] = p;
        }
    }

    void removeControlPoint(int index) {
        if (index < controlPoints.size()) {
            controlPoints.erase(controlPoints.begin() + index);
            tangentPoints.erase(tangentPoints.begin() + index);
        }
    }

    void clearControlPoints() {
        controlPoints.clear();
        tangentPoints.clear();
    }

    std::vector<int> getClosestControlPoints(float2 p) {
        closePoints.clear();
        std::vector<float2> combinedPoints;
        combinedPoints.insert(combinedPoints.end(), controlPoints.begin(), controlPoints.end());
        combinedPoints.insert(combinedPoints.end(), tangentPoints.begin(), tangentPoints.end());
        for (int i = 0; i < combinedPoints.size(); i++) {
            if ((fabs(combinedPoints[i].x - p.x) <= 0.05f && fabs(combinedPoints[i].y - p.y) <= 0.05f)) {
                closePoints.push_back(i);
            }
        }
        return closePoints;
    }

    void drawTangentPoints() {
        // draw points at control points
        glRotatef(rotationAngle, 0, 0, 1);
        glBegin(GL_POINTS);
        glColor3f(0.88f, 0.4f, 1.0f);
        for (float2 p : tangentPoints) {
            glColor3f(0.88f, 0.4f, 1.0f);
            glVertex2f(p.x, p.y);
        }
        glEnd();
        glRotatef(-rotationAngle, 0, 0, 1);
    }

    void linesBetweenTangentAndControlPoints() {
        glColor3f(0.88f, 1.0f, 1.0f);
        glLineWidth(0.1);
        glRotatef(rotationAngle, 0, 0, 1);
        glBegin(GL_LINES);
        for (int i = 0; i < controlPoints.size(); i++) {
            glVertex2f(controlPoints[i].x, controlPoints[i].y);
            glVertex2f(tangentPoints[i].x, tangentPoints[i].y);
        }
        glEnd();
        glRotatef(-rotationAngle, 0, 0, 1);

    }

    float2 hermite(int i, float t) {
        return controlPoints[i] * (2.0*t*t*t - 3.0*t*t + 1.0) + tangentPoints[i] * (t*t*t - 2.0*t*t + t) + controlPoints[i+1] * (-2.0*t*t*t + 3.0*t*t) + tangentPoints[i+1] * (t*t*t - t*t);
    }

    float2 getPoint(float t) {
        int n = int(controlPoints.size() - 1);
        int i = floor(t*n);
        float time = (t - (float(i) / float(n))) * n;
        return hermite(i, time);
    }

    float2 getDerivative(float t) {
        return float2(0.0f, 0.0f);
    }
    
    
};

#endif
