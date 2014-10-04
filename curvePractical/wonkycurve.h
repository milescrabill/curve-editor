//
//  wonkycurve.h
//  curvePractical
//
//  Created by Miles Crabill on 9/25/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_wonkycurve_h
#define curvePractical_wonkycurve_h

#include "curve.h"

class WonkyCurve : public Curve {
public:
    float scale;

    WonkyCurve(float scale = 20) {
        this->scale = scale;
    }

    float2 getPoint(float t) {
        t *= 2 * M_PI;
        float x = (16 * sin(t) * sin(t) * sin(t)) / scale;
        float y = (13 * cos(t) - 5 * cos(2*t) - 2 * cos(3*t) - cos(4*t)) / scale;
        return float2(x, y);
    }

    float2 getDerivative(float t) {
        t *= 2 * M_PI;
        float x = 12 * (cos(t) - cos(3 * t));
        float y = -13 * sin(t) + 10 * sin(2*t)+ 6 * sin(3*t) + 4 * sin(4*t);
        return float2(x,y);
    }
};

#endif
