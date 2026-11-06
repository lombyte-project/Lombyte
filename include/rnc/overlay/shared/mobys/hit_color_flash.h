#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_HIT_COLOR_FLASH_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_HIT_COLOR_FLASH_H

#include "types.h"

/* Colour flash of a hit enemy (0x10 bytes in its pvars).
 * trooper_anim_update (FUN_L00_0025d458) starts it and saves the moby's colour,
 * trooper_anim_finish (FUN_L00_0025d538) blends the moby colour toward the
 * flash colour over flash_frames, then back over return_frames. */
struct HitColorFlash {
    s16 timer;          /* 0x0: frames left in the current phase */
    s16 phase;          /* 0x2: 0 toward the flash colour, 1 back to the original */
    u8  original_red;   /* 0x4: the moby colour saved when the flash starts */
    u8  original_green; /* 0x5 */
    u8  original_blue;  /* 0x6 */
    u8  flash_red;      /* 0x7: 0 keeps the original channel */
    u8  flash_green;    /* 0x8 */
    u8  flash_blue;     /* 0x9 */
    u8  padA[2];
    s16 flash_frames;  /* 0xC */
    s16 return_frames; /* 0xE */
};

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_HIT_COLOR_FLASH_H */
