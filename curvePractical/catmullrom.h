//
//  catmullrom.h
//  curvePractical
//
//  Created by Miles Crabill on 10/19/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_catmullrom_h
#define curvePractical_catmullrom_h

#include "hermite.h"

class CatmullRom : public Hermite {
public:

    int curveType() {
        return 4;
    }

    void redistributeTangents() {
        int n = int(tangentPoints.size()) - 1;

        // if we have enough points, make the first and last tangents
        // depend on control points by looping around to the other end
        if (n > 1) {
            tangentPoints[0] = (controlPoints[1] - controlPoints[n]) * 0.5f;
            tangentPoints[n] = (controlPoints[1] - controlPoints[n-1]) * 0.5f;
        } else {
            tangentPoints[0] = float2(0.0f, 0.0f);
            tangentPoints[n] = float2(0.0f, 0.0f);
        }

        for (int i = 1; i < n; i++) {
            tangentPoints[i] = (controlPoints[i+1] - controlPoints[i-1]) * 0.5f;
        }
    }

    void addControlPoint(float2 p) {
        controlPoints.push_back(p);
        tangentPoints.push_back(p - float2(0.2f, 0.2f));
        redistributeTangents();
    }

    // overriding because have separate array of tangentpoints
    void setControlPoint(int index, float2 p) {
        if (index >= controlPoints.size()) {
            tangentPoints[index - controlPoints.size()] = p;
        } else {
            controlPoints[index] = p;
        }
        redistributeTangents();
    }

    std::vector<int> getClosestControlPoints(float2 p) {
        closePoints.clear();
        for (int i = 0; i < controlPoints.size(); i++) {
            if ((fabs(controlPoints[i].x - p.x) <= 0.05f && fabs(controlPoints[i].y - p.y) <= 0.05f)) {
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

};

#endif
