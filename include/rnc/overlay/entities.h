#ifndef LOMBYTE_RNC_OVERLAY_ENTITIES_H
#define LOMBYTE_RNC_OVERLAY_ENTITIES_H

#include "types.h"
#include "rnc/overlay/quad.h"

/*
 * Per-level table of 0x40-byte entries indexed by the moby byte at +0xA4
 * (0xFF = none), e.g. D_L08_00178900. FUN_L08_00222800 uses an entry only
 * when owner == that moby and bit 0 of unk24 is set, then stores target in
 * hero.unk2280 and aims the hero at pos (flags bit 0) or at target+0x10.
 */
typedef struct {
    u8 pad0[0x10];
    OvlQuad pos;                   /* 0x10: used when flags & 1; w == 5627.9248 is special-cased */
    u8 *target;                    /* 0x20: moby; its type (+0xA6) picks the hero state */
    s32 unk24;                     /* 0x24: bit 0 tested */
    u8 unk28;                      /* 0x28: tested == 4 */
    u8 pad29[7];
    s32 flags;                     /* 0x30: bit 0 = use pos */
    u8 *owner;                     /* 0x34: moby the entry belongs to */
    u8 pad38[8];
} OvlMobyEntry40;

typedef struct {
    char v[16];
    char padv[16];
    float f20;
    char pad24[4];
    float f28;
    char pad2C[4];
    int f30;
    int f34;
} Child;

typedef struct {
    int a, b;
} Pair;

#endif /* LOMBYTE_RNC_OVERLAY_ENTITIES_H */
