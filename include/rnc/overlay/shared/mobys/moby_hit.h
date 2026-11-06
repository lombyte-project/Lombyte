#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_MOBY_HIT_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_MOBY_HIT_H

#include "types.h"
#include "rnc/overlay/shared/math/quad.h"

/* One of the 64 hit records at D_L00_00178100 (per-level copies at other
   addresses). record_moby_hit and spawn_nearby_effect fill the next one and
   store its index in the owner moby at +0xA4; find_moby_hit and the hero's
   damage check read it back.
   attack_type / attack_subtype come from bytes 0x18 / 0x19 of the hit
   descriptor (record_moby_hit). resolve_moby_hit_response picks the reaction
   kind from the pair and scales an exact push (w 5627.9248) by the type;
   without an exact push it treats the hit as type 8. The hero doubles the upward push of type 4. Values
   the descriptors use (type / subtype): 0 enemy shots, contact and Ratchet's
   body hits (0/1); 1 blaster, suck cannon shot, turret shot (1/1, 1/3), drone
   (1/2); 2 explosions and bomb glove; 3 rockets, visibomb, the helicopter and
   Ratchet's push on classes 500..540 (3/2, 3/3); 5 tesla claw and pyrocitor
   (5/1, 5/3); 7 Giant Clank. */
typedef struct {
    OvlVec4 position;       /* 0x00 */
    OvlVec4 direction;      /* 0x10: the hero is knocked back along it when has_direction is set */
    u8     *source_moby;    /* 0x20: becomes hero.damage_source_moby */
    s32     flags;          /* 0x24: tested against the caller's mask */
    u8      attack_type;    /* 0x28: kind of attack (see above) */
    u8      attack_subtype; /* 0x29: 1..3, picks the creature reaction within a type */
    u16     source_class;   /* 0x2A: oclass of source_moby */
    f32     damage; /* 0x2C: taken off hero.vehicle_hp; a weaker hit does not replace the owner's */
    s32     has_direction; /* 0x30: set when direction is longer than 0.0001 */
    void   *owner;         /* 0x34: the moby that was hit */
    s32     unk38;
    u8      pad3C[4];
} MobyHit;

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_MOBY_HIT_H */
