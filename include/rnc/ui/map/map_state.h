#ifndef LOMBYTE_RNC_UI_MAP_MAP_STATE_H
#define LOMBYTE_RNC_UI_MAP_MAP_STATE_H

#include "types.h"

struct MapIcon;
struct MapHdr;

/* Map screen state at D_001A00F0. The per-level arrays are indexed by the
   loaded map's index (`loaded`); the five slots cache decoded maps.
   Other views of the same address in src/ (GameState, GameProgress, Level,
   UiGlobals, Hud) are merged here. */
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
    s32 level;  /* 0x224: selected level (0..19): fun_0021c4c0 steps it to the next/previous level that has missions and indexes the mission choice by it; update_mission_list reads D_001A2B70[level]; draw_map_screen shows its map and passes it to menu_system.action_value */
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
    s64 tex0;       /* 0x258: func_00204E30 result for the map image (hdr +0x10) with palette hdr +0x10 +0x420 (draw_map_screen); first argument of FUN_00200468 in fun_00220850 */
    s64 tex1;       /* 0x260: same image with palette hdr +0x14 +0x420 */
    s64 tex2;       /* 0x268: same image with palette hdr +0x18 +0x420 */
    u8 pad270[0x8];
    s32 slot[5];      /* 0x278: decoded map buffer per slot, 0 if free */
    s32 slot_id[5];   /* 0x28C: map id held by each slot, -1 if none */
    s32 sel;          /* 0x2A0 */
    s32 slot_size[5]; /* 0x2A4 */
}; /* size 0x2B8 */

#define MAP_STATE_OFFSET_CHECK(field, off) \
    typedef char map_state_offset_check_##field[ \
        ((unsigned long)&((struct MapState *)0)->field == (off)) ? 1 : -1]
MAP_STATE_OFFSET_CHECK(icons, 0x20);
MAP_STATE_OFFSET_CHECK(zoom, 0xB4);
MAP_STATE_OFFSET_CHECK(level, 0x224);
MAP_STATE_OFFSET_CHECK(loaded, 0x228);
MAP_STATE_OFFSET_CHECK(hdr, 0x23C);
MAP_STATE_OFFSET_CHECK(tex0, 0x258);
MAP_STATE_OFFSET_CHECK(slot, 0x278);
MAP_STATE_OFFSET_CHECK(slot_id, 0x28C);
MAP_STATE_OFFSET_CHECK(sel, 0x2A0);
MAP_STATE_OFFSET_CHECK(slot_size, 0x2A4);
#undef MAP_STATE_OFFSET_CHECK

extern struct MapState D_001A00F0;

#endif /* LOMBYTE_RNC_UI_MAP_MAP_STATE_H */
