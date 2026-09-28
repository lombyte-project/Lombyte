#include "types.h"
struct Tfrag { u32 tex0; u8 pad4[0x1F]; u8 tex; u8 pad24[0xC]; u32 tex1; u8 pad34[0x1C]; };
struct TfragGroup { struct Tfrag *tfrags; s32 count; };
struct TexRemap { s16 lo; s16 hi; };
extern struct TfragGroup D_001E0F00[];
extern struct TexRemap D_001E0C00[];
void patch_tfrag_gifs(void) __asm__("FUN_00233308");

void patch_tfrag_gifs(void) {
    struct TfragGroup *g;
    struct Tfrag *t;
    struct TexRemap *r;
    s32 n;
    s32 count;

    for (g = D_001E0F00; g->tfrags != 0; g++) {
        t = g->tfrags;
        count = g->count;
        for (n = 0; n < count; n++) {
            r = &D_001E0C00[t->tex];
            if (r->lo != 0) {
                t->tex0 = (t->tex0 & 0xFFFFC000) | r->lo;
            }
            if (r->hi != 0) {
                t->tex1 = (t->tex1 & 0xFFFFC000) | r->hi;
            }
            t++;
        }
    }
}

extern __typeof__(patch_tfrag_gifs) func_00233308 __attribute__((alias("FUN_00233308")));
