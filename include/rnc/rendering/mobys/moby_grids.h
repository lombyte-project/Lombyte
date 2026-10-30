#ifndef LOMBYTE_RNC_RENDERING_MOBYS_MOBY_GRIDS_H
#define LOMBYTE_RNC_RENDERING_MOBYS_MOBY_GRIDS_H

#include "types.h"

/* One cell of the moby grid. update_moby_grids (asm, 0x0020DC20) finds a
   moby's cell at cells[moby byte 0xA1][moby byte 0xA0] (x * 4 + y * 0x100),
   keeps its moby indices in block_count 32-byte blocks starting at
   first_block, and grows or shrinks the run as count changes. */
typedef struct MobyGridCell {
    u16 first_block; /* index into MobyGrids.blocks */
    u8  count;       /* moby indices stored in the run */
    u8  block_count; /* blocks in the run */
} MobyGridCell;

/* The grid and its block pool: the cells take the first 0x4000 bytes, the
   blocks of 16 moby indices start at +0x4000 (update_moby_grids and
   coll_line_fix both add 0x4000). initialize_level_render_state clears all
   0x10000 bytes with fast_mem_zero16. */
typedef struct MobyGrids {
    MobyGridCell cells[64][64];
    u16          blocks[0x600][16];
} MobyGrids;

extern MobyGrids moby_grids __asm__("D_001B76E0");

/* One bit per pool block, set while the block is in use: fun_0020dba0
   allocates a run (scans words with lwu), fun_0020db40 frees one (bytes).
   initialize_level_render_state clears its 0x60 bytes. */
extern u32 moby_grid_block_bitmap[24] __asm__("D_001C76E0");

#endif /* LOMBYTE_RNC_RENDERING_MOBYS_MOBY_GRIDS_H */
