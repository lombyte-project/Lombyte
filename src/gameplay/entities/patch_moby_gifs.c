#include "types.h"
struct GifEntry { u8 tex[12]; s32 gif; };
struct MobyClass { u8 pad0[0x20]; struct GifEntry *gifs; };
struct TexRemap { s16 lo; s16 hi; };
extern s32 D_001B6500[];
extern struct MobyClass *D_001B3200[];
extern struct TexRemap D_001B5D80[];
void patch_moby_gifs(void) __asm__("FUN_0020cef8");

void patch_moby_gifs(void) {
    s32 *id;
    struct GifEntry *e;
    u8 *tex;
    u32 *gif;
    struct TexRemap *r;

    for (id = D_001B6500; *id >= 0; id++) {
        e = D_001B3200[*id]->gifs;
        for (;; e++) {
            gif = (u32 *)(e->gif & 0x7FFFFFFF);
            tex = e->tex;
            if (*tex != 0xFF) {
                do {
                    r = &D_001B5D80[*tex];
                    if (r->lo != 0) {
                        gif[12] = (gif[12] & 0xFFFFC000) | r->lo;
                    }
                    if (r->hi != 0) {
                        gif[16] = (gif[16] & 0xFFFFC000) | r->hi;
                    }
                    tex++;
                    gif += 16;
                } while (*tex != 0xFF);
            }
            if (e->gif < 0) {
                break;
            }
        }
    }
}

extern __typeof__(patch_moby_gifs) func_0020CEF8 __attribute__((alias("FUN_0020cef8")));
