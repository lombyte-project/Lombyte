#ifndef LOMBYTE_RNC_OVERLAY_MOBY_HIT_H
#define LOMBYTE_RNC_OVERLAY_MOBY_HIT_H

#include "types.h"
#include "rnc/overlay/quad.h"

/* One of the 64 hit records at D_L00_00178100 (per-level copies at other
   addresses). record_moby_hit and spawn_nearby_effect fill the next one and
   store its index in the owner moby at +0xA4; find_moby_hit and the hero's
   damage check read it back. */
typedef struct {
    OvlVec4 position;    /* 0x00 */
    OvlVec4 direction;   /* 0x10: the hero is knocked back along it when has_direction is set */
    u8     *source_moby; /* 0x20: becomes hero.damage_source_moby */
    s32     flags;       /* 0x24: tested against the caller's mask */
    u8      unk28;
    u8      unk29;
    u16     unk2A;
    f32     damage; /* 0x2C: taken off hero.vehicle_hp; a weaker hit does not replace the owner's */
    s32     has_direction; /* 0x30: set when direction is longer than 0.0001 */
    void   *owner;         /* 0x34: the moby that was hit */
    s32     unk38;
    u8      pad3C[4];
} MobyHit;

#endif /* LOMBYTE_RNC_OVERLAY_MOBY_HIT_H */
