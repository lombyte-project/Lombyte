#include "types.h"

extern char *camera_visibility_grid __asm__("D_0015F640");

int parse_occlusion_grid(int x, int y, int z) __asm__("FUN_001f2690");

int parse_occlusion_grid(int x, int y, int z) {
    char           *grid = camera_visibility_grid;
    char           *visibility_masks = grid + *(int *)grid;
    unsigned short *axis_lookup = (unsigned short *)(grid + 4);
    int             cz, cy, cx;

    cz = z - axis_lookup[0];
    if (cz < 0)
        return 0;
    if (cz >= axis_lookup[1])
        return 0;
    if (axis_lookup[cz + 2] == 0)
        return 0;
    axis_lookup = (unsigned short *)(grid + axis_lookup[cz + 2] * 4);
    cy = y - axis_lookup[0];
    if (cy < 0)
        return 0;
    if (cy >= axis_lookup[1])
        return 0;
    if (axis_lookup[cy + 2] == 0)
        return 0;
    axis_lookup = (unsigned short *)(grid + axis_lookup[cy + 2] * 4);
    cx = x - axis_lookup[0];
    if (cx < 0 || cx >= axis_lookup[1])
        return 0;
    if (axis_lookup[cx + 2] == 0xFFFF)
        return 0;
    return (int)(visibility_masks + axis_lookup[cx + 2] * 128);
}

extern __typeof__(parse_occlusion_grid) func_001F2690 __attribute__((alias("FUN_001f2690")));
