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

class Hermite : public Freeform {
protected:
public:

    void addControlPoint(float2 p) {
        controlPoints.push_back(p);
        controlPoints.push_back(p - float2(0.2f, 0.2f));
    }

    void linesBetweenTangentAndControlPoints() {
        for (int i = 0; i < controlPoints.size(); i+=2) {
            glBegin(GL_LINES);
            glLineWidth(0.1);
            glColor3f(0.88f, 1.0f, 1.0f);
            glVertex2f(controlPoints[i].x, controlPoints[i].y);
            glVertex2f(controlPoints[i+1].x, controlPoints[i+1].y);
            glEnd();
        }
    }

    float2 hermite(int i, int n, float t) {
        return controlPoints[i] * (2.0*t*t*t - 3.0*t*t + 1.0) + controlPoints[i+1] * (t*t*t - 2.0*t*t + t) + controlPoints[i+2] * (-2.0*t*t*t + 3.0*t*t) + controlPoints[i+3] * (t*t*t - t*t);
    }

    float2 getPoint(float t) {
        float2 r(0.0, 0.0);
        int n = int(controlPoints.size() - 1);
        for (int i = 0; i < n; i+=2) {
            float time = t - (float(i) / float(n)) * n;
            r = hermite(i, n, time);
        }
        return r;
    }

    float2 getDerivative(float t) {
        return float2(0.0f, 0.0f);
    }
    
    
};

#endif
