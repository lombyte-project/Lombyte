#ifndef LOMBYTE_RNC_OVERLAY_WATER_GRID_H
#define LOMBYTE_RNC_OVERLAY_WATER_GRID_H

#include "types.h"

/* One 0x1190-byte grid of a level's water surface table (48 grids in l05,
   36 in l07), 16 units apart. */
typedef struct {
    f32 x;            /* 0x00 */
    f32 y;            /* 0x04 */
    f32 z;            /* 0x08: water height */
    u8  neighbors[8]; /* 0x0C: index of the grid 16 units away in each direction, 0xFF: none */
    u8  pad14[8];
    u8  unk1C;      /* 0x1C */
    u8  unk1D;      /* 0x1D */
    u16 draw_value; /* 0x1E */
    u8  rest[0x1170];
} WaterGrid;

extern WaterGrid l05_water_grids[] __asm__("D_L05_001D6880");
extern WaterGrid l07_water_grids[] __asm__("D_L07_001DCE40");

#endif /* LOMBYTE_RNC_OVERLAY_WATER_GRID_H */
