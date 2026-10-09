#ifndef LOMBYTE_RNC_GAMEPLAY_SURFACE_HEIGHT_GRID_H
#define LOMBYTE_RNC_GAMEPLAY_SURFACE_HEIGHT_GRID_H

#include "types.h"

typedef struct {
    u8 pad_0[8];
    f32 origin_x;
    f32 origin_y;
    f32 cell_width;
    f32 cell_height;
} SurfaceHeightGrid;

extern SurfaceHeightGrid surface_height_grid __asm__("D_001E66E0");

#endif /* LOMBYTE_RNC_GAMEPLAY_SURFACE_HEIGHT_GRID_H */
