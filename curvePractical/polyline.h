//
//  polyline.h
//  curvePractical
//
//  Created by Miles Crabill on 10/2/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_polyline_h
#define curvePractical_polyline_h

class Polyline : Freeform {
protected:
    std::vector<float2> points;
public:

    void draw() {
        glBegin(GL_LINE_STRIP);
        for (float2 p : controlPoints) {
            glVertex2f(p.x, p.y);
        }
        glEnd();
    }

//    virtual float2 getPoint(float t) = 0;
//    virtual float2 getDerivative(float t) = 0;

};


#endif
