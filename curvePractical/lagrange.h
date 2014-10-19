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

    int curveType() {
        return 1;
    }

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
    }

    void removeControlPoint(int index) {
        controlPoints.erase(controlPoints.begin() + index);
        knots.erase(knots.begin());
        redistributeKnots();
    }

    double lagrange(int i, int n, double t) {
        double numerator = 1.0f;
        double denominator = 1.0f;
        for (int j = 0; j < knots.size(); j++) {
            if (i != j) {
                numerator *= t - knots[j];
                denominator *= knots[i] - knots[j];
            }
        }
        return numerator / denominator;
    }

    float2 getPoint(float t) {
        float2 r(0.0, 0.0);
        for (int i = 0; i < controlPoints.size(); i++) {
            double l = lagrange(i, int(controlPoints.size()) - 1, t);
            double lx = controlPoints[i].x * l;
            double ly = controlPoints[i].y * l;
            r.x += lx;
            r.y += ly;
        }
        return r;
    }

    float2 getDerivative(float t) {
        return float2(0.0f, 0.0f);
    }


};

#endif
