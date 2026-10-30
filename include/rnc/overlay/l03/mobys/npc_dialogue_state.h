#ifndef LOMBYTE_RNC_OVERLAY_L03_MOBYS_NPC_DIALOGUE_STATE_H
#define LOMBYTE_RNC_OVERLAY_L03_MOBYS_NPC_DIALOGUE_STATE_H

#include "types.h"

/* The dialogue record of the l03 talking mobys (helga.c, big_al.c) that initialize_npc_dialogue
   and update_npc_dialogue run. */
struct TalkState {
    u8    pad0[4];
    s16   mode; /* 0x04: 1 talked, 2 talked and accepted */
    u8    pad6[0x1A];
    void *script; /* 0x20 */
    u8    pad24[0x12];
    s16   unk36;
    u8    pad38[0x10];
    s32   unk48;
    s32   warp; /* 0x4C: index into l03_cuboids, -1 for none */
};

#endif /* LOMBYTE_RNC_OVERLAY_L03_MOBYS_NPC_DIALOGUE_STATE_H */
