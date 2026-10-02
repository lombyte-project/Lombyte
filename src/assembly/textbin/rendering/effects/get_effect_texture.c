#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/effects/get_effect_texture/FUN_001f44b8.s", FUN_001f44b8);
#else
#include "types.h"

struct EffectEntry {
    s64 handle;
    u16 tile_x;
    u16 tile_y;
    s16 off;
    s16 size;
};

struct TexEntry { s32 data; s16 flags; s16 cbp; s32 clut; u8 tw; u8 th; s16 tbp; };

extern s32 D_0015EE74;
extern s32 D_0015F458;
extern s32 D_0015F460;
extern struct TexEntry D_0018D040[];
extern struct EffectEntry D_0018D440[];

s64 get_effect_texture(s32 index) __asm__("FUN_001f44b8");

s64 get_effect_texture(s32 index) {
    struct EffectEntry *e;
    struct TexEntry *d;
    s32 offset;
    s32 shift;
    s32 cbp;
    s32 base;
    s32 tbp;
    s64 desc;

    e = &D_0018D440[index];
    if (e->handle == 0) {
        offset = e->off;
        shift = offset - 6;
        cbp = D_0015EE74 >> 8;
        base = D_0015EE74 + 0x400;
        tbp = base >> 8;
        desc = tbp | ((u64)(1 << ((shift <= -1) ? 0 : shift)) << 14)
             | ((u64)(u16)e->off << 26 | 0x1300000)
             | ((u64)(u16)e->size << 30)
             | ((u64)cbp << 37 | (u64)1 << 34)
             | (u64)1 << 63;
        D_0015EE74 = base + (1 << (offset + e->size));
        e->handle = desc;
        if (D_0015F458 < 0x40) {
            d = &D_0018D040[D_0015F458];
            d->cbp = cbp;
            d->data = D_0015F460 + e->tile_y * 0x10;
            d->flags = 0;
            *(s32 *)((u8 *)D_0018D040 + D_0015F458 * 0x10 + 8) = D_0015F460 + e->tile_x * 0x10;
            d->tw = (u8)e->off;
            d->th = (u8)e->size;
            d->tbp = tbp;
            D_0015F458 += 1;
        }
    }
    return D_0018D440[index].handle;
}
#endif /* NON_MATCHING */
