//
//  freeformcurve.h
//  curvePractical
//
//  Created by Miles Crabill on 9/29/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_freeformcurve_h
#define curvePractical_freeformcurve_h

#include "curve.h"
#include <vector>

class Freeform : public Curve {
protected:
    std::vector<float2> controlPoints;
    std::vector<int> closePoints;
public:
    
    virtual float2 getPoint(float t) = 0;
    virtual float2 getDerivative(float t) = 0;
    virtual int curveType() = 0;

    virtual float2 getControlPoint(int index) {
        return controlPoints[index];
    }

    virtual void setControlPoint(int index, float2 p) {
        controlPoints[index] = p;
    }

    virtual void addControlPoint(float2 p) {
        controlPoints.push_back(p);
    }

    virtual void removeControlPoint(int index) {
        controlPoints.erase(controlPoints.begin() + index);
    }

    virtual void clearControlPoints() {
        controlPoints.clear();
    }

    int numberOfControlPoints() {
        return int(controlPoints.size());
    }

    virtual std::vector<int> getClosestControlPoints(float2 p) {
        closePoints.clear();
        for (int i = 0; i < controlPoints.size(); i++) {
            if ((fabs(controlPoints[i].x - p.x) <= 0.05f && fabs(controlPoints[i].y - p.y) <= 0.05f)) {
                closePoints.push_back(i);
            }
        }
        return closePoints;
    }

    void drawControlPoints(){
        // draw points at control points
        glRotatef(rotationAngle, 0, 0, 1);
        glBegin(GL_POINTS);
        for (float2 p : controlPoints) {
            glVertex2f(p.x, p.y);
        }
        glEnd();
        glRotatef(-rotationAngle, 0, 0, 1);
    }


    virtual void fill() {
        glRotatef(rotationAngle, 0, 0, 1);
        glBegin(GL_POLYGON);
        for (float2 p : points) {
            glVertex2f(p.x, p.y);
        }
        glEnd();
        glRotatef(-rotationAngle, 0, 0, 1);
    }
};

#endif
