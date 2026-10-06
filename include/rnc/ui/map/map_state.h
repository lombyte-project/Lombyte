#ifndef LOMBYTE_RNC_UI_MAP_MAP_STATE_H
#define LOMBYTE_RNC_UI_MAP_MAP_STATE_H

#include "types.h"

struct MapIcon;
struct MapHdr;

/* Map screen state at D_001A00F0. The per-level arrays are indexed by the
   loaded map's index (`loaded`); the five slots cache decoded maps. */
struct MapState {
    u8 pad0[0xC];
    s32 unkC; /* 0x00C */
    u8 pad10[0x10];
    struct MapIcon *icons; /* 0x020: icon list of the current level */
    s32 unk24;             /* 0x024: zero while the map cannot move */
    u8 pad28[0x8C];
    f32 zoom[20];  /* 0x0B4 */
    s32 pan_x[20]; /* 0x104 */
    s32 pan_y[20]; /* 0x154 */
    u8 pad1A4[0x80];
    s32 cur;    /* 0x224: map id shown */
    s32 loaded; /* 0x228: index of the loaded map, -1 none */
    s32 unk22C;
    s32 unk230;
    s32 unk234;
    u8 pad238[0x4];
    struct MapHdr *hdr; /* 0x23C */
    s32 unk240;
    s32 unk244;
    s32 unk248;
    s32 unk24C;
    s32 unk250;
    u8 pad254[0x4];
    s64 unk258;
    s64 unk260;
    s64 unk268;
    u8 pad270[0x8];
    s32 slot[5];      /* 0x278: decoded map buffer per slot, 0 if free */
    s32 slot_id[5];   /* 0x28C: map id held by each slot, -1 if none */
    s32 sel;          /* 0x2A0 */
    s32 slot_size[5]; /* 0x2A4 */
}; /* size 0x2B8 */

extern struct MapState D_001A00F0;

#endif /* LOMBYTE_RNC_UI_MAP_MAP_STATE_H */
