#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MATH_INTERPOLATION_COMPUTE_DIRECTED_ANGLE_DELTA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MATH_INTERPOLATION_COMPUTE_DIRECTED_ANGLE_DELTA_H

#include "types.h"

float approach_angle(void *cur,
                     void *vel,
                     float target,
                     float lim,
                     float accel,
                     float maxv) __asm__("FUN_L00_0025be00");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MATH_INTERPOLATION_COMPUTE_DIRECTED_ANGLE_DELTA_H */
