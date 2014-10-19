//
//  bezierclock.h
//  curvePractical
//
//  Created by Miles Crabill on 10/18/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_bezierclock_h
#define curvePractical_bezierclock_h

#include "bezierdigit.h"
#include <vector>

class BezierClock {
public:
    std::vector<BezierDigit> digits;

    BezierClock() {
        digits[0] = BezierDigit(0);
        digits[1] = BezierDigit(1);
        digits[2] = BezierDigit(2);
        digits[3] = BezierDigit(3);
        digits[4] = BezierDigit(4);
        digits[5] = BezierDigit(5);
        digits[6] = BezierDigit(6);
        digits[7] = BezierDigit(7);
        digits[8] = BezierDigit(8);
        digits[9] = BezierDigit(9);
    }
};

#endif
