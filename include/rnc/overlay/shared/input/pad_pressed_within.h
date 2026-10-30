#ifndef LOMBYTE_RNC_OVERLAY_SHARED_INPUT_PAD_PRESSED_WITHIN_H
#define LOMBYTE_RNC_OVERLAY_SHARED_INPUT_PAD_PRESSED_WITHIN_H

#include "types.h"

s32 pad_pressed_within(s32, s32, void *) __asm__("FUN_L00_00266d60");
s32 check_pad_press_separation(s32, void *, void *, s32) __asm__("FUN_L00_00266e00");
f32 read_pad_float_history(s32) __asm__("FUN_L00_00266e80");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_INPUT_PAD_PRESSED_WITHIN_H */
