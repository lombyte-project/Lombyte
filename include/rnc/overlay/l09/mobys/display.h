#ifndef LOMBYTE_RNC_OVERLAY_L09_MOBYS_DISPLAY_H
#define LOMBYTE_RNC_OVERLAY_L09_MOBYS_DISPLAY_H

#include "rnc/math/vector.h"
#include "types.h"

struct Moby;

void display_update(struct Moby *) __asm__("FUN_L09_002edc30");
struct Moby *
spawn_tumbling_debris(struct Moby *, Vec4 *, Vec4 *, s32, f32) __asm__("FUN_L09_002ef868");

#endif /* LOMBYTE_RNC_OVERLAY_L09_MOBYS_DISPLAY_H */
