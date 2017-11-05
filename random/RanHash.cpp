/*
 * RanHash.cpp
 * This file is part of graphSampler
 *
 * Copyright (C) 2017 - Giacomo Bergami
 * Created by Giacomo Bergami on 04/04/17.
 *
 * graphSampler is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * graphSampler is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with graphSampler. If not, see <http://www.gnu.org/licenses/>.
 */

 


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
