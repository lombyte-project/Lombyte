#ifndef LOMBYTE_RNC_OVERLAY_HUD_H
#define LOMBYTE_RNC_OVERLAY_HUD_H

#include "types.h"

typedef struct {
    char pad0[0x8];
    int unk08;
    int *unk0C;
    char pad10[0x34];
    int icon;           /* 0x44: icon sprite id */
    short unk48;
    short unk4A;
    char pad4C[0x4];
    int unk50;
    int unk54;
    int w;
    int h;
    int flags;
    char pad64[0x8];
    int unk6C;
    unsigned char cnt[4];
    int unk74;
    int unk78;
    int unk7C;
    void *unk80;
} HudElem;

#endif /* LOMBYTE_RNC_OVERLAY_HUD_H */
