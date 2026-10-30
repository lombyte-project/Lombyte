/* Ported from rac1-decomp (src/game/tiefunc.c, func_00236CA8). */
#include "sda.h"
#include "rnc/rendering/draw_config.h"
#include "rnc/math/asm_helpers.h"
#define RENDER_PACKET_CURSOR_ATTR MACRO_ADDR
#include "rnc/rendering/dma_tag.h"
#include "types.h"
#include "rnc/rendering/render_globals.h"
#include "rnc/globals.h"

extern void FlushCache(int);

#include "rnc/rendering/object_render_class.h"

extern s32            D_001E3000[];
extern u8             D_001E4200[];
extern u8             D_001E2A00[];
extern u8             D_001E3E00[];
extern u8             tie_light_data2[] __asm__("D_001E4400");
extern int D_00160F68 MACRO_ADDR;
extern u8             D_00160F30[];
extern u8             D_00160F40[];
extern u8             D_001DF030[];
extern void           noop_core_callback_secondary(void *,
                                                   int) __asm__("func_001F21B8"); /* empty profiling marker */
extern void noop_core_callback(void *, int) __asm__("func_001F21B0"); /* empty profiling marker */
extern void tie_proc(void) __asm__("FUN_00235be8");
extern void WriteDmaChannel(void *, int, int);
extern void dma_tie_textures(void) __asm__("func_00235640"); /* DmaTieTextures */
extern int  registered_object_render_class_count_macro __asm__("D_00160F4C") MACRO_ADDR;
/* DrawTies_2: DrawTies_1's two passes, the first with the odd ties' 0x8
   flag set (and the saved tie data swapped in around it), the second with
   the even ones'. Each flag loop reads the tie count into its own local
   (the loop's stores could alias it) and the flag is a `short` (so the
   `&= ~8` stays an int AND with -9, not andi 0xFFF7). */
void draw_ties_2(void) __asm__("FUN_00235990");

void draw_ties_2(void) {
    {
        int i;
        int n = registered_object_render_class_count_macro;
        for (i = 1; i < n; i += 2) {
            object_render_classes[i]->flags |= 8;
        }
    }
    {
        int p = render_packet_cursor.addr;
        D_00160F68 = p;
        gs_texture_allocation_cursor = gs_texture_allocation_start;
        p += 0x10;
        render_packet_cursor.addr = p;
    }
    noop_core_callback_secondary(D_00160F30, 1);
    {
        struct DrawConfig *d = &draw_config;
        if (d->tie.enabled != 0) {
            FlushCache(0);
            tie_proc();
            WriteDmaChannel(tie_light_data2, 0x3600, 0x40);
        }
    }
    dma_tie_textures();
    fast_mem_copy(D_001E4200, D_001E3000, 0x200);
    fast_mem_copy(D_001E3E00, D_001E2A00, 0x400);
    {
        int i;
        int n = registered_object_render_class_count_macro;
        for (i = 1; i < n; i += 2) {
            object_render_classes[i]->flags &= ~8;
        }
    }
    {
        int i;
        int n = registered_object_render_class_count_macro;
        for (i = 0; i < n; i += 2) {
            object_render_classes[i]->flags |= 8;
        }
    }
    {
        struct DrawConfig *d = &draw_config;
        int                p = render_packet_cursor.addr;
        D_00160F68 = p;
        gs_texture_allocation_cursor = gs_texture_allocation_start;
        p += 0x10;
        render_packet_cursor.addr = p;
        if (d->tie.enabled != 0) {
            FlushCache(0);
            tie_proc();
            WriteDmaChannel(tie_light_data, 0x3600, 0x40);
        }
    }
    noop_core_callback_secondary(D_00160F40, 5);
    dma_tie_textures();
    {
        int i;
        int n = registered_object_render_class_count_macro;
        for (i = 0; i < n; i += 2) {
            object_render_classes[i]->flags &= ~8;
        }
    }
    fast_mem_copy((void *)render_packet_cursor.addr, D_001DF030, 0x20);
    noop_core_callback(D_00160F40, 5);
}

extern __typeof__(draw_ties_2) func_00235990 __attribute__((alias("FUN_00235990")));
