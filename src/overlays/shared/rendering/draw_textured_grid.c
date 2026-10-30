/* Draws a moby as a textured grid: builds its interpolated packet, uploads the scratchpad index table and restores the view. */
#include "types.h"
#include "qcopy.h"

extern char textured_grid_scratchpad_index_table[] __asm__("D_L03_001CAF00");
extern void FUN_L02_0024f748(char *);
extern void FUN_L02_0024fa80(char *);
extern void build_interpolated_dma_packet(char *) __asm__("FUN_L02_002a40f0");
extern void restore_view_after_textured_grid(char *) __asm__("FUN_L02_002a46e0");
extern void FUN_L02_002101b8(u32, u32, u32);

/* Ported from rac1-decomp (src/overlays/shared/vendor_00292AC0.c: func_L03_00292AC0), where it is exact; names translated to the US level program. */

void draw_textured_grid(char *moby) __asm__("FUN_L03_00291918");

void draw_textured_grid(char *moby) {
    build_interpolated_dma_packet(moby);
    FUN_L02_002101b8(textured_grid_scratchpad_index_table, 0x70002800, 0xF8);
    FUN_L02_0024f748(moby);
    FUN_L02_0024fa80(moby);
    restore_view_after_textured_grid(moby);
}
