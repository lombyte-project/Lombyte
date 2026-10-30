#ifndef LOMBYTE_RNC_OVERLAY_SHARED_PROPS_POOL_SET_SCALE_H
#define LOMBYTE_RNC_OVERLAY_SHARED_PROPS_POOL_SET_SCALE_H

#include "types.h"

void pool_set_scale(f32) __asm__("FUN_L01_002b7ef0");
void pool_flush(void *, s32) __asm__("FUN_L01_002b7fe0");
void pool_ripple(void *, f32, f32, f32, void *, s32) __asm__("FUN_L01_002b8288");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_PROPS_POOL_SET_SCALE_H */
