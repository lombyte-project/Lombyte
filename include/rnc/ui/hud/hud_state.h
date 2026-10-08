#ifndef LOMBYTE_RNC_UI_HUD_HUD_STATE_H
#define LOMBYTE_RNC_UI_HUD_HUD_STATE_H

#include "types.h"

/* HUD sprite, animation and texture state at D_0019A3E8. The arrays at
   0x1C..0x28 point into the HUD bank file whose header sits at 0x18
   (load_hud_banks sets them from the header's offsets). */

/* One HUD animation (0x1C): `start` indexes the frame references. */
struct HudAnimDef {
    u16 id;
    u16 count;
    u16 start;
    u8 flags;
    u8 pad7;
};

/* One frame (0x20): palette page (0x28) and image page (0x24) indices. */
struct HudFrameRef {
    s16 palette_index;
    s16 image_index;
};

/* One palette or image page (0x24, 0x28). Bit 31 of source_address is
   set until link_hud_bank relocates the bank. */
struct HudTexPage {
    u32 source_address;
    u16 gs_block_offset;
    u8 width_log2;
    u8 height_log2;
};

/* Bank file header, page counts view (link_hud_bank, hud_send_resident_bank). */
struct HudTexCounts {
    u8 pad0[0x14];
    s32 mid_ends[8];
    s32 ends[16];
    s32 loaded[16];
};

/* Bank file header, load view (load_hud_banks). */
struct HudBank {
    s32 unk0;
    s32 off4;
    s32 off8;
    s32 offC;
    s32 off10;
    u8 pad14[0x40];
    s32 has54;
    s32 size58;
    s32 has5C;
    s32 has60;
    s32 has64;
    u8 pad68[0x2C];
    s32 unk94;
    s32 unk98;
    s32 unk9C;
    s32 unkA0;
    s32 unkA4;
};

struct HudState {
    s32 serial; /* 0x00: next animation serial (queue_animation_update) */
    s32 unk4;
    s32 unk8;
    s32 z;        /* 0x0C: sprite Z in the high word of XYZ */
    s32 heap_cur; /* 0x10: hud_heap_alloc cursor */
    s32 heap_end; /* 0x14 */
    union {
        struct HudTexCounts *counts;
        struct HudBank *bank;
    } header;                          /* 0x18 */
    struct HudAnimDef *anim_defs;      /* 0x1C */
    struct HudFrameRef *frame_refs;    /* 0x20 */
    struct HudTexPage *image_pages;    /* 0x24 */
    struct HudTexPage *palette_pages;  /* 0x28 */
    s32 unk2C;
    s32 unk30;
};

/* Compile-time layout checks: a wrong offset makes the array size negative. */
#define HUD_STATE_OFFSET_CHECK(field, off) \
    typedef char hud_state_offset_check_##field[ \
        ((unsigned long)&((struct HudState *)0)->field == (off)) ? 1 : -1]
HUD_STATE_OFFSET_CHECK(z, 0xC);
HUD_STATE_OFFSET_CHECK(heap_cur, 0x10);
HUD_STATE_OFFSET_CHECK(header, 0x18);
HUD_STATE_OFFSET_CHECK(anim_defs, 0x1C);
HUD_STATE_OFFSET_CHECK(frame_refs, 0x20);
HUD_STATE_OFFSET_CHECK(image_pages, 0x24);
HUD_STATE_OFFSET_CHECK(palette_pages, 0x28);
HUD_STATE_OFFSET_CHECK(unk30, 0x30);

extern struct HudState hud_state __asm__("D_0019A3E8");

#endif /* LOMBYTE_RNC_UI_HUD_HUD_STATE_H */
