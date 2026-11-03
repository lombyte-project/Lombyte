/* Ported from rac1-decomp (src/game/vendor.c, func_0023B018). */
#include "sda.h"
#include "rnc/rendering/render_globals.h"

typedef struct {
    int            x;
    int            y;
    int            z;
    unsigned short w;
    unsigned short h;
} ZoneBox;

#include "rnc/world/collision/surface_height_grid.h"

extern SurfaceHeightTile *surface_height_tiles_macro __asm__("D_00161190") MACRO_ADDR;

extern int truncate_float_to_s32(float) __asm__("func_001FA6D0");
/* Index of the zone whose box (in 1/1024 units, 0x800 deep) holds the
   point and whose tile's cell_mask has the point's cell set, or -1. Cells
   are sized and offset by the grid in surface_height_grid. */
int find_zone_at_point(float x, float y, float z) __asm__("FUN_00239d60");

int find_zone_at_point(float x, float y, float z) {
    int      ix = truncate_float_to_s32(x * 1024.0f);
    int      iy = truncate_float_to_s32(y * 1024.0f);
    int      iz = truncate_float_to_s32(z * 1024.0f);
    ZoneBox *zone_bounds = occl_cell_table;
    int      i;

    for (i = 0; i < occl_cell_count; i++, zone_bounds++) {
        SurfaceHeightTile *tile;
        int                cx;
        int                cy;
        int                bit;

        if (ix < zone_bounds->x || iy < zone_bounds->y || iz < zone_bounds->z) {
            continue;
        }
        if (ix >= zone_bounds->x + zone_bounds->w || iy >= zone_bounds->y + zone_bounds->h ||
            iz >= zone_bounds->z + 0x800) {
            continue;
        }
        tile = &surface_height_tiles_macro[i];
        cx = truncate_float_to_s32((x - (tile->x + surface_height_grid.origin_x)) /
                                   surface_height_grid.cell_width);
        cy = truncate_float_to_s32((y - (tile->y + surface_height_grid.origin_y)) /
                                   surface_height_grid.cell_height);
        bit = 1 << (((cx >> 2) & 3) | (cy & 0xC));
        if (tile->cell_mask & bit) {
            return i;
        }
    }
    return -1;
}

extern __typeof__(find_zone_at_point) func_00239D60 __attribute__((alias("FUN_00239d60")));

s32 surface_height_tiles = 0;

s32 occl_cell_table = 0;

s32 occl_cell_count = 0;
