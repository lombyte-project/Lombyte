#ifndef LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_CREATE_PART_H
#define LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_CREATE_PART_H

#include "types.h"
#include "rnc/math/vector.h"

/* One 0x40-byte slot of the part table. The first 0x20 bytes are the same for
   every part type; the rest belong to the type's update function. */
struct EffectParticle {
    u8  type;  /* 0x0: part type create_part was given */
    u8  flags; /* 0x1: 0x40 marks a slot from the second allocation bitmap (free_effect_particle) */
    u8  texture; /* 0x2 */
    u8  blend;   /* 0x3: 0x44 or 0x48 */
    s32 color;   /* 0x4: 0xAABBGGRR */
    u8  spin;    /* 0x8 */
    u8  unk9;
    s16 life; /* 0xA: frames left (fast_dec_timer_s16) */
    f32 size; /* 0xC: a two-point part keeps its second colour here */
    Vec4 pos; /* 0x10 */
    u8   data[0x20];
};

void *create_part(s32) __asm__("FUN_L00_002678b8");
void  free_effect_particle(void *) __asm__("FUN_L00_00267a08");

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_RENDERING_EFFECTS_CREATE_PART_H */
