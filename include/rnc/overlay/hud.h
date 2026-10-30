#ifndef LOMBYTE_RNC_OVERLAY_HUD_H
#define LOMBYTE_RNC_OVERLAY_HUD_H

#include "types.h"

/* One of the 13 HUD element records at D_L00_0017DC50. update_hud_elements
   counts unk7C down each frame (unk04 bit 0x10 holds it up), moves unk6C
   between -6 and 30 with it and calls update; hud_keep_visible finds a
   record by key. */
typedef struct HudElem {
    int  unk00;
    int  unk04;
    int  unk08;
    int *unk0C;
    char pad10[0x4];
    void (*update)(struct HudElem *);
    char          pad18[0x8];
    int           unk20;
    int           unk24;
    char          pad28[0x1C];
    int           icon; /* 0x44: icon sprite id */
    short         unk48;
    short         unk4A;
    char          pad4C[0x4];
    int           unk50;
    int           unk54;
    int           w;
    int           h;
    int           flags;
    int           key;
    int           unk68;
    int           unk6C;
    unsigned char cnt[4];
    int           unk74;
    int           unk78;
    int           unk7C;
    void         *unk80;
    char          pad84[0xC];
} HudElem;

extern HudElem hud_elements[13] __asm__("D_L00_0017DC50") __attribute__((section(".data")));

#endif /* LOMBYTE_RNC_OVERLAY_HUD_H */
