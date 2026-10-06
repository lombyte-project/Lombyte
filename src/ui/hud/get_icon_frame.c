#include "types.h"
struct AnimDef {
    u16 id;
    u16 count;
    u16 start;
    u8 flags;
    u8 pad7;
};
struct FramePair {
    s16 a;
    s16 b;
};
struct FrameInfo {
    s32 word;
    s32 pad;
};
struct AnimTable {
    s32 serial;
    u8 pad4[0x18];
    struct AnimDef *defs;
    struct FramePair *pairs;
    struct FrameInfo *infoB;
    struct FrameInfo *infoA;
};
extern struct AnimTable D_0019A3E8;
extern s32 hud_get_icon_index(s32) __asm__("func_001FEE38");
s32 get_icon_frame(s32 id, s32 frame) __asm__("FUN_001ff960");

s32 get_icon_frame(s32 id, s32 frame) {
    s32 i = hud_get_icon_index(id);
    s32 k;

    if (D_0019A3E8.defs[i].id == 0xFFFF) {
        return 0;
    }
    if (frame >= D_0019A3E8.defs[i].count) {
        return 0;
    }
    k = D_0019A3E8.defs[i].start + frame;
    if (D_0019A3E8.infoA[D_0019A3E8.pairs[k].a].word & 0x80000000) {
        return 0;
    }
    if (D_0019A3E8.infoB[D_0019A3E8.pairs[k].b].word & 0x80000000) {
        return 0;
    }
    return k;
}

extern __typeof__(get_icon_frame) func_001FF960
    __attribute__((alias("FUN_001ff960")));
