//
//  bezierclock.h
//  curvePractical
//
//  Created by Miles Crabill on 10/18/14.
//  Copyright (c) 2014 Miles Crabill. All rights reserved.
//

#ifndef curvePractical_bezierclock_h
#define curvePractical_bezierclock_h

#include "beziernumber.h"
#include <vector>

class BezierClock {
public:
    std::vector<BezierNumber> digits;

    BezierClock() {
        digits[0] = BezierNumber(0);
        digits[1] = BezierNumber(1);
        digits[2] = BezierNumber(2);
        digits[3] = BezierNumber(3);
        digits[4] = BezierNumber(4);
        digits[5] = BezierNumber(5);
        digits[6] = BezierNumber(6);
        digits[7] = BezierNumber(7);
        digits[8] = BezierNumber(8);
        digits[9] = BezierNumber(9);
    }
};

#endif
