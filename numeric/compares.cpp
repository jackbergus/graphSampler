//
// Created by Giacomo Bergami on 18/08/16.
//

#include "compares.h"

void compareU_Int(unsigned int left, unsigned int right, int *toret) {
    *toret = (left==right) ? 0 : (left<right ? -1 : 1);
}

void compareUL_Int(unsigned long a, unsigned long b, int *res) {
    if (a<b) *res = -1;
    else if (a>b) *res = 1;
    else *res=0;
}