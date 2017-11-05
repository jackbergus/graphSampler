//
// Created by Giacomo Bergami on 22/08/16.
//


#include <iostream>
#include "RanHash.h"

unsigned long RanHash::int64(unsigned long jump) {
    unsigned long v = jump * 3935559000370003845L + 2691343689449507681L;
    v ^= v >> 21;
    v ^= v << 37;
    v ^= v >> 4;
    v *= 4768777513237032717L;
    v ^= v << 20;
    v ^= v >> 41;
    v ^= v << 5;
    return v;
}

double RanHash::doub(unsigned long jump) {
    return 5.42101086242752217E-20 * (double)RanHash::int64(jump);
}
