#ifndef LOMBYTE_RNC_UI_MAP_MAP_STATE_H
#define LOMBYTE_RNC_UI_MAP_MAP_STATE_H

#include "types.h"

struct MapIcon;
struct MapMarker;
struct MapHdr;

/*
 * Map screen state at D_001A00F0. Other views of the same address in src/
 * (GameState, GameProgress, Level, UiGlobals, Hud, TextTable,
 * MapMarkerState) are merged here.
 *
 * - level: fun_0021c4c0 steps it to the next/previous level with missions;
 *   update_mission_list reads D_001A2B70[level]; draw_map_screen shows that
 *   map and passes it as menu_system.unkE4.
 * - zoom/pan_x/pan_y are indexed by `loaded`; slot[] caches five decoded
 *   maps (find_map_entry_slot).
 * - mask: compose_bitmap_from_mask input when the shown level is the
 *   current one; fun_00207b08 / FUN_00208030 pack it into the level's
 *   0x800-byte save page (cleared instead when unk28 is zero).
 * - unk22C / unk234: stash_receive_data arguments when the next map is the
 *   current level's (draw_map_screen); unk234 is stored as slot_size.
 * - The *_vram words are GS memory byte addresses (>> 8 = block) passed to
 *   FUN_00204e30 as CBP (arg 5) and TBP0 (arg 6). tex0..tex2 are the TEX0
 *   values of the 128x128 images at hdr +0x10/+0x14/+0x18 (CLUT +0x20,
 *   pixels +0x420), all drawn with tex0's CLUT; tex0 is also drawn by
 *   fun_00220850 and draw_transition_overlay.
 */
struct MapState {
    u8 pad0[0x8];
    s32 unk8;                  /* 0x008: draw_map_overlay draft tests it with unk2C */
    u8 *mask;                  /* 0x00C: 512x512 1-bit explored mask of the current level */
    u8 pad10[0x4];
    s32 unk14;                 /* 0x014: passed with mask to func_001FA860 */
    s32 z;                     /* 0x018: GS Z of the map quads (draw_map_overlay packets) */
    struct MapMarker *markers; /* 0x01C: drawn by draw_map_markers, may be null */
    struct MapIcon *icons;     /* 0x020: icon list of the shown level */
    s32 unk24;                 /* 0x024: zero: update_map_zoom_and_pan and draw_map_overlay return early */
    s32 unk28;                 /* 0x028: zero: the save page is cleared, not packed */
    s32 unk2C;                 /* 0x02C: see unk8 */
    struct {
        s32 unk0;              /* draw_map_overlay draft: <0 skipped */
        u8 pad4[0xC];
    } unk30[8];                /* 0x030 */
    s32 marker_count;          /* 0x0B0: entries in markers */
    f32 zoom[20];              /* 0x0B4: per-map zoom, 0.65..4 */
    s32 pan_x[20];             /* 0x104 */
    s32 pan_y[20];             /* 0x154 */
    u8 pad1A4[0x80];
    s32 level;                 /* 0x224: selected level 0..19 */
    s32 loaded;                /* 0x228: index of the loaded map, -1 none */
    s32 unk22C;                /* 0x22C: stash_receive_data argument */
    s32 unk230;                /* 0x230: compared with -current_level_index (draw_map_screen) */
    s32 unk234;                /* 0x234: stash_receive_data byte count */
    u8 pad238[0x4];
    struct MapHdr *hdr;        /* 0x23C */
    s32 map_image_vram;        /* 0x240: TBP0 of the 512x512 composed map */
    s32 tex_clut_vram;         /* 0x244: CBP shared by tex0..tex2 */
    s32 tex0_vram;             /* 0x248: TBP0 of tex0 */
    s32 tex1_vram;             /* 0x24C: TBP0 of tex1 */
    s32 tex2_vram;             /* 0x250: TBP0 of tex2 */
    u8 pad254[0x4];
    s64 tex0;                  /* 0x258: TEX0 of the image at hdr +0x10 */
    s64 tex1;                  /* 0x260: hdr +0x14 */
    s64 tex2;                  /* 0x268: hdr +0x18 */
    u8 pad270[0x8];
    s32 slot[5];               /* 0x278: decoded map buffer per slot, 0 if free */
    s32 slot_id[5];            /* 0x28C: map id held by each slot, -1 if none */
    s32 sel;                   /* 0x2A0: slot being filled, -1 none */
    s32 slot_size[5];          /* 0x2A4 */
}; /* size 0x2B8 */

extern struct MapState D_001A00F0;

#endif /* LOMBYTE_RNC_UI_MAP_MAP_STATE_H */
