#ifndef LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_MOBY_TARGET_INFO_H
#define LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_MOBY_TARGET_INFO_H

#include "types.h"

/* The target info record get_moby_target_info returns (pvars[0] of a moby with
   flag 0x20). Enemies keep it at pvars+0x20, so their pvars structs spell the
   same fields at +0x20: health 0x20, hit_weight 0x28, wrench_hit_sound 0x29,
   size 0x2A, aim_height 0x30, look_range 0x58, look_priority 0x59 and
   look_height 0x5A.

   Readers: resolve_moby_hit_response (hit_weight, last_hit_kind, hit_kind,
   hit_cooldown), select_auto_aim_target_moby (size, aim_height), the blaster,
   suck cannon and doom bot (aim_height), get_wrench_hit_sound_variant
   (wrench_hit_sound) and update_hero_target_tracking (look_*). */
typedef struct MobyTargetInfo {
    f32 health;           /* 0x00 */
    u8  pad4[4];          /* 0x04 */
    u8  hit_weight;       /* 0x08: hit response table column, knockback scale row */
    u8  wrench_hit_sound; /* 0x09: 1 picks the wrench's second hit sound */
    u8  size;             /* 0x0A: radius in eighths, for auto-aim */
    u8  padB[5];          /* 0x0B */
    f32 aim_height;       /* 0x10: height above the moby position that shots aim at */
    s16 last_hit_kind;    /* 0x14: kind of the last hit; a different kind clears hit_cooldown */
    s16 hit_kind;         /* 0x16: kind of this frame's hit, -1 for none */
    u8  pad18[4];         /* 0x18 */
    s16 hit_cooldown;  /* 0x1C: while it runs, repeat hits of the same kind get a weaker response */
    u16 flags;         /* 0x1E: 0x80 set on the moby a suck cannon shot is aimed at */
    u8  pad20[0x18];   /* 0x20 */
    u8  look_range;    /* 0x38: Ratchet looks at the moby from this close */
    u8  look_priority; /* 0x39: the highest priority in range wins */
    u8  look_height;   /* 0x3A: eighths above the moby position that Ratchet looks at */
} MobyTargetInfo;

#endif /* LOMBYTE_RNC_OVERLAY_SHARED_MOBYS_MOBY_TARGET_INFO_H */
