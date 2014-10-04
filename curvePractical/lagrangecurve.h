//
//  lagrangecurve.h
//  curvePractical
//
//  Created by Miles Crabill on 10/2/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_lagrangecurve_h
#define curvePractical_lagrangecurve_h

#include "curve.h"

class LagrangeCurve : public Freeform {
protected:
    std::vector<float> knots;
public:
    void redistributeKnots() {
        for (int i = 0; i < knots.size(); i++) {
            knots[i] = i * (1 / pow(2, knots.size()));
        }
    }

    void addControlPoint(float2 p) {
        controlPoints.push_back(p);
        knots.push_back(0.0f);
        redistributeKnots();
    }

    float2 getPoint(float t) {
        float x = 0.0f;
        float y = 0.0f;

        double numerator = 1.0f;
        double denominator = 1.0f;

        for (int i = 0; i < controlPoints.size(); i++) {
            for (int j = 0; j < controlPoints.size(); j++) {
                if (i != j) {
                    numerator *= t - controlPoints[j].x;
                    denominator *= controlPoints[i].x - controlPoints[j].x;
                }
            }
            y = controlPoints[i].y * (numerator / denominator);
        }
        printf("t: %f, y: %f\n", x, y);
        return float2(t, y);
    }

    float2 getDerivative(float t) {
        return float2(0.0f, 0.0f);
    }


};

#endif
