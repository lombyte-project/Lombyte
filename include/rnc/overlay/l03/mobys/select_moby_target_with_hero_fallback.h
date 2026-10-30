#ifndef LOMBYTE_RNC_OVERLAY_L03_MOBYS_SELECT_MOBY_TARGET_WITH_HERO_FALLBACK_H
#define LOMBYTE_RNC_OVERLAY_L03_MOBYS_SELECT_MOBY_TARGET_WITH_HERO_FALLBACK_H

#include "types.h"
#include "rnc/math/vector.h"

/* What select_moby_target_with_hero_fallback finds within range of a moby
   (homing_rocket.c, mine_layer.c). */
typedef struct {
    Vec4 aim; /* 0x00: point to aim at (the mine layer turns toward it) */
    u8   pad10[0x10];
    Vec4 pos; /* 0x20 */
    u8   pad30[0x10];
    s32  found; /* 0x40 */
    u8   pad44[0xC];
} MobyTargetSelection;

#endif /* LOMBYTE_RNC_OVERLAY_L03_MOBYS_SELECT_MOBY_TARGET_WITH_HERO_FALLBACK_H */
