/* Ported from rac1-decomp (src/game/vendor.c, func_0023B018). */
#include "sda.h"
typedef struct {
    int x;
    int y;
    int z;
    unsigned short w;
    unsigned short h;
} ZoneBox;
extern ZoneBox *D_00161194 MACRO_ADDR;
extern char *D_00161190 MACRO_ADDR;
extern int D_00161198 MACRO_ADDR;
extern float D_001E66E0[];
extern int truncate_float_to_s32(float) __asm__("func_001FA6D0");
/* Index of the zone whose box (in 1/1024 units, 0x800 deep) holds the
   point and whose 4x4 cell mask at +0x1E has the point's cell set, or
   -1. Cells are sized and offset by the grid in D_001E66E0. */
int find_zone_at_point(float x, float y, float z) __asm__("FUN_00239d60");

int find_zone_at_point(float x, float y, float z) {
    int ix = truncate_float_to_s32(x * 1024.0f);
    int iy = truncate_float_to_s32(y * 1024.0f);
    int iz = truncate_float_to_s32(z * 1024.0f);
    ZoneBox *b = D_00161194;
    int i;

    for (i = 0; i < D_00161198; i++, b++) {
        char *zn;
        int cx;
        int cy;
        int bit;

        if (ix < b->x || iy < b->y || iz < b->z) {
            continue;
        }
        if (ix >= b->x + b->w || iy >= b->y + b->h || iz >= b->z + 0x800) {
            continue;
        }
        zn = D_00161190 + i * 0x1190;
        cx = truncate_float_to_s32((x - (*(float *)(zn + 0) + D_001E66E0[2])) / D_001E66E0[4]);
        cy = truncate_float_to_s32((y - (*(float *)(zn + 4) + D_001E66E0[3])) / D_001E66E0[5]);
        bit = 1 << (((cx >> 2) & 3) | (cy & 0xC));
        if (*(unsigned short *)(zn + 0x1E) & bit) {
            return i;
        }
    }
    return -1;
}

extern __typeof__(find_zone_at_point) func_00239D60 __attribute__((alias("FUN_00239d60")));
