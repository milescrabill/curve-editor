//
//  bezier.h
//  curvePractical
//
//  Created by Miles Crabill on 9/29/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_bezier_h
#define curvePractical_bezier_h

#include "freeformcurve.h"

class BezierCurve : public Freeform {
public:

    static double bernstein(int i, int n, double t) {
        if(n == 1) {
            if(i == 0) return 1-t;
            if(i == 1) return t;
            return 0;
        }
        if(i < 0 || i > n) return 0;
        return (1 - t) * bernstein(i, n-1, t) + t * bernstein(i-1, n-1, t);
    }

    static double bernsteinDerivative(int i, int n, double t) {
        if(n == 1) {
            if(i == 0) return -1;
            if(i == 1) return 1;
            return 0;
        }
        if(i < 0 || i > n) return 0;
        return -1.0 * bernstein(i, n-1, t) + (1 - t) * bernsteinDerivative(i, n-1, t) + bernstein(i-1, n-1, t) + t * bernsteinDerivative(i-1, n-1, t);
    }

    float2 getPoint(float t) {
        float2 r(0.0, 0.0);
        for (int i = 0; i < controlPoints.size(); i++) {
            double b = bernstein(i, int(controlPoints.size()) - 1, t);
            double bx = controlPoints[i].x * b;
            double by = controlPoints[i].y * b;
            r.x += bx;
            r.y += by;
        }
        return r;
    }

    float2 getDerivative(float t) {
        float2 r(0.0f, 0.0f);
        for (int i = 0; i < controlPoints.size(); i++) {
            double db = bernsteinDerivative(i, int(controlPoints.size()) - 1, t);
            double dbx = controlPoints[i].x * db;
            double dby = controlPoints[i].y * db;
            r.x += dbx;
            r.y += dby;
        }
        return r;
    }

};

#endif
