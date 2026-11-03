#ifndef LOMBYTE_RNC_WORLD_COLLISION_SURFACE_HEIGHT_GRID_H
#define LOMBYTE_RNC_WORLD_COLLISION_SURFACE_HEIGHT_GRID_H

#include "types.h"

typedef struct {
    u8  pad_0[8];
    f32 origin_x;
    f32 origin_y;
    f32 cell_width;
    f32 cell_height;
} SurfaceHeightGrid;

/* One occlusion zone's box, in 1/1024 units and 0x800 deep. */
typedef struct {
    int            x;
    int            y;
    int            z;
    unsigned short w;
    unsigned short h;
} ZoneBox;

typedef struct {
    f32 samples[16][16];
    f32 pad_400[0x20];
    f32 bottom_edge[16];
    f32 right_edge[16];
    f32 pad_500[0x23];
    f32 corner;
    f32 pad_590[0xC];
} SurfaceHeightLayer;

/* One zone's height tile (0x1190 bytes), indexed like the zones of
   find_zone_at_point. */
typedef struct {
    f32                x;
    f32                y;
    f32                z;
    u8                 pad_C[0x12];
    u16                cell_mask; /* 0x1E: which of the 4x4 cells belong to the zone */
    u8                 pad_20[0x30];
    SurfaceHeightLayer layers[3];
} SurfaceHeightTile;

extern SurfaceHeightGrid surface_height_grid __asm__("D_001E66E0");
/* The level's zones: occl_cell_count boxes and one height tile each. */
extern SurfaceHeightTile *surface_height_tiles __asm__("D_00161190");
extern ZoneBox           *occl_cell_table __asm__("D_00161194");
extern s32                occl_cell_count __asm__("D_00161198");

#endif /* LOMBYTE_RNC_WORLD_COLLISION_SURFACE_HEIGHT_GRID_H */
