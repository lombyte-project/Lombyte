#ifndef LOMBYTE_RNC_OVERLAY_WATER_GRID_H
#define LOMBYTE_RNC_OVERLAY_WATER_GRID_H

#include "types.h"

/* One 0x1190-byte grid of a level's water surface table (48 grids in l05,
   36 in l07, 20 in l11), 16 units apart. */
typedef struct {
    f32 x;                 /* 0x00 */
    f32 y;                 /* 0x04 */
    f32 z;                 /* 0x08: water height */
    u8  neighbors[8];      /* 0x0C: index of the grid 16 units away in each direction, 0xFF: none */
    u32 texture_id;        /* 0x14 */
    u32 second_texture_id; /* 0x18 */
    u8  alpha_2_fix;       /* 0x1C: FIX of the GS ALPHA_2 register */
    u8  alpha_1_fix;       /* 0x1D: FIX of the GS ALPHA_1 register */
    u16 visible_subtiles;  /* 0x1E: bit i = sub-tile i of the 4x4 grid (row i >> 2, column i & 3)
                              that render_water_grids draws; 0: grid not drawn */
    u8  rest[0x1170];
} WaterGrid;

extern WaterGrid l05_water_grids[48] __asm__("D_L05_001D6880");
extern WaterGrid l07_water_grids[36] __asm__("D_L07_001DCE40");
extern WaterGrid l11_water_grids[20] __asm__("D_L11_001DA880");

#endif /* LOMBYTE_RNC_OVERLAY_WATER_GRID_H */
