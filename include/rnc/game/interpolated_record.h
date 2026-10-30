#ifndef LOMBYTE_RNC_GAME_INTERPOLATED_RECORD_H
#define LOMBYTE_RNC_GAME_INTERPOLATED_RECORD_H

#include "types.h"

struct InterpolatedStateEntry;

/* The current value of an interpolated record. */
f32 compute_interpolated_record_value(void *anim) __asm__("FUN_0020c9e0");

#endif /* LOMBYTE_RNC_GAME_INTERPOLATED_RECORD_H */
