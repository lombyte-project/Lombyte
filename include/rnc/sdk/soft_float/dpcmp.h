#ifndef LOMBYTE_RNC_SDK_SOFT_FLOAT_DPCMP_H
#define LOMBYTE_RNC_SDK_SOFT_FLOAT_DPCMP_H

#include "types.h"

s32 CompareDoubles(double, double) __asm__("dpcmp");

#endif /* LOMBYTE_RNC_SDK_SOFT_FLOAT_DPCMP_H */
