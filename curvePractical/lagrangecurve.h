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
        if (knots.size() == 1) {
            knots[0] = 1;
        } else {
            for (int i = 0; i < knots.size(); i++) {
                knots[i] = (i) * 1.0 / (knots.size() - 1);
            }
        }
    }

    void addControlPoint(float2 p) {
        controlPoints.push_back(p);
        knots.push_back(0.0f);
        redistributeKnots();
//        for (int i = 0; i < knots.size(); i++) {
//            printf("knots[%d] = %f\n",i, knots[i]);
//        }
    }

    void removeControlPoint(int index) {
        controlPoints.erase(controlPoints.begin() + index);
        knots.erase(knots.begin());
        redistributeKnots();
    }

    float2 getPoint(float t) {
        float2 p = float2(0.0f, 0.0f);

        for (int i = 0; i < controlPoints.size(); i++) {
            double numerator = 1.0f;
            double denominator = 1.0f;
            for (int j = 0; j < knots.size(); j++) {
                if (i != j) {
                    numerator *= t - knots[j];
                    denominator *= knots[i] - knots[j];
                }
            }
            p += controlPoints[i] * (numerator / denominator);
        }

        return p;
    }

    float2 getDerivative(float t) {
        return float2(0.0f, 0.0f);
    }


};

#endif
