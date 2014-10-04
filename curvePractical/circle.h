//
//  circle.h
//  curvePractical
//
//  Created by Miles Crabill on 9/18/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_circle_h
#define curvePractical_circle_h

#include "curve.h"
#include <math.h>

class Circle : public Curve::Curve {
public:
    float2 center;
    float radius;

    Circle() {
        center = float2(0.0f, 0.0f);
        radius = 1.0f;
    }

    // constructor to initialize members
    Circle(float2 c, float r) {
        center = c;
        radius = r;
    }

    float2 getDerivative(float t) {
        float x = -1.0f * (2 * M_PI * radius) * sin(2 * M_PI * t);
        float y = 2 * M_PI * radius * cos(2 * M_PI * t);
        return float2(x, y);
    }

    float2 secondDerivative(float t) {
        float x = -4.0f * M_PI*M_PI * radius * cos(M_PI*t);
        float y = -4.0f * M_PI*M_PI * radius * sin(M_PI*t);
        return float2(x, y);
    }

    float2 getPoint(float t) {
        // parametric equation
        float x = center.x + radius * cos(t * 2 * M_PI);
        float y = center.y + radius * sin(t * 2 * M_PI);
        return float2(x, y);
    }
};

#endif

