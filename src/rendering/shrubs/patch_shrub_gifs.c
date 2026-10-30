#include "types.h"
#include "rnc/rendering/tex_remap.h"
#include "rnc/rendering/shrub_render_class.h"

struct GifPacket {
    s32 count;
    s32 skip;
    u8  pad8[8];
};

extern s32             shrub_gif_patch_class_ids[] __asm__("D_001D8DB0");
extern struct TexRemap shrub_paging_table[] __asm__("D_001D88B0");
void                   patch_shrub_gifs(void) __asm__("FUN_00228a30");

void patch_shrub_gifs(void) {
    s32              *id;
    ShrubRenderClass *tc;
    s32               i;
    s32               j;
    struct GifPacket *pkt;
    u8               *gif;
    struct TexRemap  *texture_remap;

    for (id = shrub_gif_patch_class_ids; *id >= 0; id++) {
        tc = shrub_render_classes[*id];
        for (i = 0; i < tc->group_count; i++) {
            pkt = (struct GifPacket *)tc->groups[i].group + 1;
            gif = (u8 *)(pkt + pkt->skip) + 0x10;
            for (j = 0; j < pkt->count; j++) {
                texture_remap = &shrub_paging_table[gif[0x13]];
                if (texture_remap->lo != 0) {
                    *(u32 *)(gif + 0x30) = (*(u32 *)(gif + 0x30) & 0xFFFFC000) | texture_remap->lo;
                }
                if (texture_remap->hi != 0) {
                    *(u32 *)(gif + 0x20) = (*(u32 *)(gif + 0x20) & 0xFFFFC000) | texture_remap->hi;
                }
                gif += 0x40;
            }
        }
    }
}

extern __typeof__(patch_shrub_gifs) func_00228A30 __attribute__((alias("FUN_00228a30")));
