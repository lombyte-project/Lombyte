#ifndef LOMBYTE_RNC_MATH_MATRIX3X4_H
#define LOMBYTE_RNC_MATH_MATRIX3X4_H

#include "eetypes.h"

/* Three 16-byte matrix columns. */
struct Matrix3x4 {
    u128 columns[3];
};

#endif /* LOMBYTE_RNC_MATH_MATRIX3X4_H */
