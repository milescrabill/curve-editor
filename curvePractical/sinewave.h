//
//  sinewave.h
//  curvePractical
//
//  Created by Miles Crabill on 10/7/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_sinewave_h
#define curvePractical_sinewave_h

#include "float2.h"
#include <math.h>

class Sinewave {
public:
    int x, y;


    double k;

    float2 getPoint(float t) {
        return float2(t + x + k / 100, .5 * sin(t * 20) + y);
    }

    void draw() {
//        printf("k: %d", k);
        glBegin(GL_LINE_STRIP);
        // draw a line strip of small segments along the curve
        for (float i = -10; i < 10; i += 0.01f) {
            glVertex2f(getPoint(i).x, getPoint(i).y);
        }
        glEnd();
    }

    Sinewave() {
        this->k = 0;
        this->x = 0;
        this->y = 0;
    }

    Sinewave(int x, int y, double k) {
        this->k = k;
        this->x = x;
        this->y = y;
    }

};

#endif
