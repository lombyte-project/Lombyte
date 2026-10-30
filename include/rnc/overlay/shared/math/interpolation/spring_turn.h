#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MATH_INTERPOLATION_SPRING_TURN_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MATH_INTERPOLATION_SPRING_TURN_H

#include "types.h"

f32 spring_turn(void *, f32, void *, f32, f32, void *) __asm__("FUN_L00_00258110");
s32 get_first_moby_in_group(void *, s32, s32, s32) __asm__("FUN_L00_002591d0");
s32 solve_quadratic_equation(f32 *, f32 *, f32, f32, f32) __asm__("FUN_L00_002595a0");
s32 get_next_moby_in_group(void *, void *, s32, s32) __asm__("FUN_L00_002592b8");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MATH_INTERPOLATION_SPRING_TURN_H */
