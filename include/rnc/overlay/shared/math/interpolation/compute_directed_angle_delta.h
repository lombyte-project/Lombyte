#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MATH_INTERPOLATION_COMPUTE_DIRECTED_ANGLE_DELTA_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MATH_INTERPOLATION_COMPUTE_DIRECTED_ANGLE_DELTA_H

#include "types.h"

float approach_angle(void *cur,
                     void *vel,
                     float target,
                     float lim,
                     float accel,
                     float maxv) __asm__("FUN_L00_0025be00");

unsigned int swap_packed_color_channels(unsigned int, int) __asm__("FUN_L00_0025c0e8");
void         swap_color_channel_words(int *, int *, int *, int) __asm__("FUN_L00_0025c088");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MATH_INTERPOLATION_COMPUTE_DIRECTED_ANGLE_DELTA_H */
