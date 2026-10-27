#include "types.h"
#include "rnc/rendering/tex_remap.h"
struct Tfrag {
    u32 tex0;
    u8 pad4[0x1F];
    u8 tex;
    u8 pad24[0xC];
    u32 tex1;
    u8 pad34[0x1C];
};
struct TfragGroup {
    struct Tfrag *tfrags;
    s32 count;
};
extern struct TfragGroup D_001E0F00[];
extern struct TexRemap D_001E0C00[];
void patch_tfrag_gifs(void) __asm__("FUN_00233308");

void patch_tfrag_gifs(void) {
    struct TfragGroup *current_tfrag_group;
    struct Tfrag *current_tfrag;
    struct TexRemap *texture_remap;
    s32 n;
    s32 count;

    for (current_tfrag_group = D_001E0F00; current_tfrag_group->tfrags != 0; current_tfrag_group++) {
        current_tfrag = current_tfrag_group->tfrags;
        count = current_tfrag_group->count;
        for (n = 0; n < count; n++) {
            texture_remap = &D_001E0C00[current_tfrag->tex];
            if (texture_remap->lo != 0) {
                current_tfrag->tex0 = (current_tfrag->tex0 & 0xFFFFC000) | texture_remap->lo;
            }
            if (texture_remap->hi != 0) {
                current_tfrag->tex1 = (current_tfrag->tex1 & 0xFFFFC000) | texture_remap->hi;
            }
            current_tfrag++;
        }
    }
}

extern __typeof__(patch_tfrag_gifs) func_00233308 __attribute__((alias("FUN_00233308")));
