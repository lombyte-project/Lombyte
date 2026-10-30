#ifndef LOMBYTE_RNC_OVERLAY_L14_MOBYS_PUFF_H
#define LOMBYTE_RNC_OVERLAY_L14_MOBYS_PUFF_H

/* The smoke puff particle that spawn_puff (FUN_L00_0026d000) returns, as the
   level 14 enemy fighter, rocket strike fighter, rocket, minibomber and flying debris code sets it up. */

#include "types.h"

typedef struct {
    u8  pad0[4];
    s32 mode; /* 0x04 */
    u8  pad8[2];
    u8  tint; /* 0x0A */
    u8  fade; /* 0x0B */
} L14PuffTail;

typedef struct {
    u8          pad0[3];
    u8          alpha; /* 0x03 */
    u8          pad4[4];
    u8          shade; /* 0x08 */
    s8          frame; /* 0x09 */
    s16         life;  /* 0x0A */
    u8          padC[0x14];
    L14PuffTail tail; /* 0x20 */
} L14Puff;

L14Puff *spawn_puff(void *, f32, f32, f32, int, void *, f32, int) __asm__("FUN_L00_0026d000");

#endif /* LOMBYTE_RNC_OVERLAY_L14_MOBYS_PUFF_H */
