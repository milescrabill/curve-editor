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

    std::vector<float2> isPointOnCurve(float2 p) {
        std::vector<float2> closePoints;
        for (int i = 0; i < controlPoints.size() - 1; i++) {

            // compare slopes of controlPoints[i] -> mouse click
            // and slope of mouse -> controlPoints[i+1]
            float m1 = (p.y - controlPoints[i].y) / (p.x - controlPoints[i].x);
            float m2 = (controlPoints[i+1].y - p.y) / (controlPoints[i+1].x - p.x);
            printf("m1 (cp[%d]->mouse): %f, m2 (mouse[cp[%d]): %f\n", i, m1, i+1, m2);
            if (fabs(m1 - m2) - 0.05f < 0.0f) {
                closePoints.push_back(points[i]);
            }
        }
        return closePoints;
    }

    float2 getPointOnLine(int i, int n, float t) {
        float m;
        if (i <= n) {
            m = (float(controlPoints[i+1].y - controlPoints[i].y) / float(controlPoints[i+1].x - controlPoints[i].x));
        } else {
            m = (float(controlPoints[0].y - controlPoints[i].y) / float(controlPoints[0].x - controlPoints[i].x));
        }
        return float2(controlPoints[i].x + (m * t), controlPoints[i].y + (m * t));
    }

    float2 getPoint(float t) {
        int n = int(controlPoints.size() - 1);
        int i = floor(t*n);
        float time = (t - (float(i) / float(n))) * n;
        return getPointOnLine(i, n, time);
    }

    float2 getDerivative(float t) {
        return float2(0.0f, 0.0f);
    }

};


#endif
