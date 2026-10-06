#include "types.h"

extern char *D_0015F640;

int parse_occlusion_grid(int x, int y, int z) __asm__("FUN_001f2690");

int parse_occlusion_grid(int x, int y, int z) {
    char *grid = D_0015F640;
    char *base = grid + *(int *)grid;
    unsigned short *l = (unsigned short *)(grid + 4);
    int cz, cy, cx;

    cz = z - l[0];
    if (cz < 0)
        return 0;
    if (cz >= l[1])
        return 0;
    if (l[cz + 2] == 0)
        return 0;
    l = (unsigned short *)(grid + l[cz + 2] * 4);
    cy = y - l[0];
    if (cy < 0)
        return 0;
    if (cy >= l[1])
        return 0;
    if (l[cy + 2] == 0)
        return 0;
    l = (unsigned short *)(grid + l[cy + 2] * 4);
    cx = x - l[0];
    if (cx < 0 || cx >= l[1])
        return 0;
    if (l[cx + 2] == 0xFFFF)
        return 0;
    return (int)(base + l[cx + 2] * 128);
}

extern __typeof__(parse_occlusion_grid) func_001F2690 __attribute__((alias("FUN_001f2690")));
