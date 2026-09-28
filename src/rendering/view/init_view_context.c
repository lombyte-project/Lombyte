#include "types.h"

struct Display { u8 pad0[0x150]; s16 w; s16 h; };
struct Screen { s32 w; s32 h; s32 hw; s32 hh; s32 x0; s32 y0; s32 x1; s32 y1; };
struct View {
    u8 pad0[0xA0]; f32 unkA0; f32 unkA4; u8 padA8[8]; f32 unkB0; u8 padB4[0x14C];
    f32 hw; f32 hh; f32 sx; f32 sy; u8 pad210[8]; s32 unk218; f32 unk21C; u8 pad220[8];
    f32 unk228; s32 unk22C;
};

extern struct Display D_00151780;
extern struct Screen D_0013E500;
extern struct View D_0018CD00;
extern f32 func_001FA6C0(s32);

void init_view_context(void) __asm__("FUN_001f2c60");

void init_view_context(void) {
    struct Display *d = &D_00151780;
    struct View *v = &D_0018CD00;
    s32 hw;
    s32 hh;

    hw = d->w >> 1;
    hh = d->h >> 1;
    D_0013E500.w = d->w;
    D_0013E500.h = d->h;
    D_0013E500.hw = hw;
    D_0013E500.hh = hh;
    D_0013E500.x0 = (0x800 - hw) << 4;
    D_0013E500.y0 = (0x800 - hh) << 4;
    D_0013E500.x1 = (hw + 0x800) << 4;
    D_0013E500.y1 = (hh + 0x800) << 4;
    v->unkA0 = 32.0f;
    v->unkA4 = 745472.0f;
    v->unkB0 = 0.63f;
    v->hw = func_001FA6C0(d->w) * 0.5f;
    v->hh = func_001FA6C0(d->h) * 0.5f;
    v->sx = v->hw * 4.0f;
    v->sy = v->hh * 4.0f;
    v->unk21C = 524288.0f;
    v->unk228 = 255.0f;
    v->unk218 = 0;
    v->unk22C = 0;
}

extern __typeof__(init_view_context) func_001F2C60 __attribute__((alias("FUN_001f2c60")));
