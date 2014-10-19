//
//  polyline.h
//  curvePractical
//
//  Created by Miles Crabill on 10/2/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_polyline_h
#define curvePractical_polyline_h

class Polyline : public Freeform {
public:

    int curveType() {
        return 2;
    }

    void draw() {
        glLoadIdentity();
        glPushMatrix();
        glRotatef(rotationAngle, 0, 0, 1);
        glBegin(GL_LINE_STRIP);
        // draw a line strip of small segments along the curve
        for (int i = 0; i < controlPoints.size(); i++) {
            glVertex2f(controlPoints[i].x, controlPoints[i].y);
        }
        glEnd();
        glPopMatrix();
    }

    void fill() {
        glRotatef(rotationAngle, 0, 0, 1);
        glBegin(GL_POLYGON);
        for (float2 p : controlPoints) {
            glVertex2f(p.x, p.y);
        }
        glEnd();
        glRotatef(-rotationAngle, 0, 0, 1);

    }

    bool isBetween(float2 a, float2 b, float2 c) {
        float cp = (c.y - a.y) * (b.x - a.x) - (c.x - a.x) * (b.y - a.y);
        if (fabs(cp) > 0.05){
            return false;
        }

        float dp = (c.x - a.x) * (b.x - a.x) + (c.y - a.y)*(b.y - a.y);
        if (dp < 0) {
            return false;
        }

        float lengthSquared = (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y);
        if (dp > lengthSquared) {
            return false;
        }

        return true;
    }

    std::vector<float2> isPointOnCurve(float2 p) {
        std::vector<float2> closePoints;
        for (int i = 0; i < controlPoints.size() - 1; i++) {
            if (isBetween(controlPoints[i], controlPoints[i+1], p)) {
                closePoints.push_back(points[i]);
            }
        }
        return closePoints;
    }

    float2 getPoint(float t) {
        return float2(0.0f, 0.0f);
    }

    float2 getDerivative(float t) {
        return float2(0.0f, 0.0f);
    }

};


#endif
