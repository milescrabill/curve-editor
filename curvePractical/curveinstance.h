//
//  curveinstance.h
//  curvePractical
//
//  Created by Miles Crabill on 10/9/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_curveinstance_h
#define curvePractical_curveinstance_h

class CurveInstance {
protected:
    Curve* curve;
    float2 position;
public:
    CurveInstance(Curve* curve, float2 position)
        :curve(curve), position(position) {}

    CurveInstance() {}

    void setPosition(float2 p) {
        position = p;
    }

    float2 getPosition() {
        return position;
    }

    void draw() {
        if (curve != NULL) {
            glTranslatef(position.x, position.y, 1);
            curve->draw();
            glTranslatef(-position.x, -position.y, 1);
        }
    }
};

#endif
