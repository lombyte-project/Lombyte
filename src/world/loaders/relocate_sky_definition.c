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
extern s16 log2dim(s32) __asm__("FUN_001f97a0");

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

void relocate_sky_definition(struct SpriteFile *sky_definition) {
    s32 *serialized_texture_words;
    s32 i;
    s32 k;
    s32 j;
    s32 serialized_palette_offset;
    s32 serialized_texel_offset;
    s32 texture_width;
    s32 texture_height;
    struct Group *relocated_group;

    D_0016045C = sky_definition;
    sky_definition->loaded = 1;
    sky_definition->sprites += (s32)sky_definition;
    sky_definition->unk14 += (s32)sky_definition;
    sky_definition->unk18 += (s32)sky_definition;
    if (sky_definition->unk1C != 0) {
        sky_definition->unk1C += (s32)sky_definition;
    }
    serialized_texture_words = (s32 *)D_0016045C->sprites;
    for (i = 0; i < D_0016045C->nsprites; i++) {
        serialized_palette_offset = *serialized_texture_words++;
        serialized_texel_offset = *serialized_texture_words++;
        texture_width = *serialized_texture_words++;
        texture_height = *serialized_texture_words++;
        ((struct Sprite *)D_0016045C->sprites)[i].y = serialized_palette_offset >> 4;
        ((struct Sprite *)D_0016045C->sprites)[i].x = serialized_texel_offset >> 4;
        ((struct Sprite *)D_0016045C->sprites)[i].w = log2dim(texture_width);
        ((struct Sprite *)D_0016045C->sprites)[i].h = log2dim(texture_height);
        ((struct Sprite *)D_0016045C->sprites)[i].unk0 = 0;
    }
    for (k = 0; k < D_0016045C->ngroups; k++) {
        D_0016045C->groups[k] += (s32)sky_definition;
        relocated_group = (struct Group *)D_0016045C->groups[k];
        for (j = 0; j < relocated_group->count; j++) {
            ((struct Group *)((u8 *)relocated_group + j * 0x20))->items[0] += (s32)sky_definition;
        }
    }
}

extern __typeof__(relocate_sky_definition) func_002028E0 __attribute__((alias("FUN_002028e0")));
