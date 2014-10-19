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
    void redistributeTangents() {
        int n = int(controlPoints.size()) - 1;

        // if we have enough points, make the first and last tangents
        // depend on control points by looping around to the other end
        if (n > 1) {
            tangentPoints[0] = (controlPoints[1] - controlPoints[n]) * 0.5f;
            tangentPoints[n] = (controlPoints[1] - controlPoints[n-1]) * 0.5f;
        } else {
            tangentPoints[0] = float2(0.0f, 0.0f);
            tangentPoints[n] = float2(0.0f, 0.0f);
        }

        for (int i = 1; i < tangentPoints.size() - 1; i++) {
            tangentPoints[i] = (controlPoints[i+1] - controlPoints[i-1]) * 0.5f;
        }
    }

    void addControlPoint(float2 p) {
        controlPoints.push_back(p);
        tangentPoints.push_back(p - float2(0.2f, 0.2f));
        redistributeTangents();
    }

    // combines tangentPoints and controlPoints into one vector
    // so i don't have to duplicate a ton of code
    void combinePoints() {
        combinedPoints.clear();
        combinedPoints.insert(combinedPoints.end(), controlPoints.begin(), controlPoints.end());
        combinedPoints.insert(combinedPoints.end(), tangentPoints.begin(), tangentPoints.end());
    }

    // overriding because have separate array of tangentpoints
    // based on index we know which array we're accessing from
    // because of how getClosestControlPoints returns indices
    float2 getControlPoint(int index) {
        if (index >= controlPoints.size()) {
            combinePoints();
            return combinedPoints[index];
        } else {
            return controlPoints[index];
        }
    }

    // overriding because have separate array of tangentpoints
    void setControlPoint(int index, float2 p) {
        if (index >= controlPoints.size()) {
//            tangentPoints[index - controlPoints.size()] = p;
        } else {
            controlPoints[index] = p;
        }
        redistributeTangents();
    }

    void removeControlPoint(int index) {
        controlPoints.erase(controlPoints.begin() + index);
        tangentPoints.erase(tangentPoints.begin() + index);
    }

    void clearControlPoints() {
        controlPoints.clear();
        tangentPoints.clear();
    }

    std::vector<int> getClosestControlPoints(float2 p) {
        closePoints.clear();
        std::vector<float2> combinedPoints;
        combinedPoints.insert(combinedPoints.end(), controlPoints.begin(), controlPoints.end());
        combinedPoints.insert(combinedPoints.end(), tangentPoints.begin(), tangentPoints.end());
        for (int i = 0; i < combinedPoints.size(); i++) {
            if ((fabs(combinedPoints[i].x - p.x) <= 0.05f && fabs(combinedPoints[i].y - p.y) <= 0.05f)) {
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
