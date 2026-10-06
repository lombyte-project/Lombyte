#include "types.h"
#include "rnc/rendering/dma_tag.h"
struct Screen {
    u8 pad[0x10];
    s32 offx;
    s32 offy;
};
struct HudIndex {
    s16 unk0;
    s16 slot;
};
struct HudTex {
    u8 pad[6];
    u8 wlog;
    u8 hlog;
};
struct HudState {
    u8 pad[0xC];
    s32 z;
    u8 pad10[0x10];
    struct HudIndex *index;
    struct HudTex *tex;
};
extern struct TagPtr D_00160F00;
extern struct Screen D_0013E500;
extern struct HudState D_0019A3E8;
extern u64 get_frame_texture(s32) __asm__("func_001FFA10");

void draw_hud_sprite_uv(s32 id, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v,
                        s32 alpha) __asm__("FUN_00200258");

void draw_hud_sprite_uv(s32 id, s32 x, s32 y, s32 w, s32 h, s32 u, s32 v, s32 alpha) {
    struct HudState *hud = &D_0019A3E8;
    struct HudTex *t;
    s32 tw;
    s32 th;
    u64 *q;

    t = &hud->tex[hud->index[id].slot];
    tw = 1 << (t->wlog + 4);
    th = 1 << (t->hlog + 4);
    D_00160F00.p->tag = 0x10000005;
    D_00160F00.p->addr = 0;
    D_00160F00.p->vif0 = 0;
    D_00160F00.p->vif1 = 0x50000005;
    q = (u64 *)++D_00160F00.p;
    q[0] = (u64)0xE800 << 47 | 0x8001;
    q[1] = 0x5353106;
    q[2] = get_frame_texture(id);
    q[3] = 0x156;
    q[4] = (u64)alpha << 24 | 0x7F7F7F;
    q[5] = u | ((u64)v << 16);
    q[6] = (x + D_0013E500.offx - 8) | ((u64)(y + D_0013E500.offy - 8) << 16) | ((u64)hud->z << 32);
    q[7] = (u + tw) | ((u64)(v + th) << 16);
    q[8] = (x + w + D_0013E500.offx - 8) | ((u64)(y + h + D_0013E500.offy - 8) << 16) |
           ((u64)hud->z << 32);
    q[9] = 0;
    D_00160F00.p += 5;
}

extern __typeof__(draw_hud_sprite_uv) func_00200258 __attribute__((alias("FUN_00200258")));
