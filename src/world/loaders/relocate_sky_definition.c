#include "types.h"

struct Sprite {
    s64 unk0;
    s16 x;
    s16 y;
    s16 w;
    s16 h;
};

struct Group {
    s32 count;
    u8 pad4[0x1C];
    s32 items[1];
};

struct SpriteFile {
    u8 pad0[4];
    s16 loaded;
    s16 ngroups;
    u8 pad8[4];
    s16 nsprites;
    u8 padE[2];
    s32 sprites;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 groups[1];
};

extern struct SpriteFile *D_0016045C;
extern s16 FUN_001f97a0(s32);

/* Prepares the sky data of a level. The sky block starts with a header:
 *   0x00  background colour (RGBA), set only on some levels
 *   0x04  a flag that is set on two levels
 *   0x06  number of shells (sky layers)
 *   0x0c  number of textures
 *   0x10  offset of the texture list (palette, pixels, width, height)
 *   0x14  offset of the texture pixel data
 *   0x20  offsets of each shell
 * Each shell starts with its number of clusters. The word at shell + 4 says
 * the shell has no texture and uses vertex colours; only shell 0 does that. */
void relocate_sky_definition(struct SpriteFile *f) __asm__("FUN_002028e0");

void relocate_sky_definition(struct SpriteFile *f) {
    s32 *src;
    s32 i;
    s32 k;
    s32 j;
    s32 a;
    s32 b;
    s32 c;
    s32 d;
    struct Group *g;

    D_0016045C = f;
    f->loaded = 1;
    f->sprites += (s32)f;
    f->unk14 += (s32)f;
    f->unk18 += (s32)f;
    if (f->unk1C != 0) {
        f->unk1C += (s32)f;
    }
    src = (s32 *)D_0016045C->sprites;
    for (i = 0; i < D_0016045C->nsprites; i++) {
        a = *src++;
        b = *src++;
        c = *src++;
        d = *src++;
        ((struct Sprite *)D_0016045C->sprites)[i].y = a >> 4;
        ((struct Sprite *)D_0016045C->sprites)[i].x = b >> 4;
        ((struct Sprite *)D_0016045C->sprites)[i].w = FUN_001f97a0(c);
        ((struct Sprite *)D_0016045C->sprites)[i].h = FUN_001f97a0(d);
        ((struct Sprite *)D_0016045C->sprites)[i].unk0 = 0;
    }
    for (k = 0; k < D_0016045C->ngroups; k++) {
        D_0016045C->groups[k] += (s32)f;
        g = (struct Group *)D_0016045C->groups[k];
        for (j = 0; j < g->count; j++) {
            ((struct Group *)((u8 *)g + j * 0x20))->items[0] += (s32)f;
        }
    }
}

extern __typeof__(relocate_sky_definition) func_002028E0 __attribute__((alias("FUN_002028e0")));
