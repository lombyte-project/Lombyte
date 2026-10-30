/* Pool: initializes the height tiles and binds a tile. */
#include "types.h"
#include "asm.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/math/asm_helpers.h"

/* Converts a float vector to fixed-point ints, relative to the grid origin, plus two scale shorts. */
/* Ported from rac1-decomp (src/overlays/shared/vuchain_002B8C00.c: func_L01_002B8E20), where it is exact; names translated to the US level program. */

void pool_bind_tile(float *in, int *out) __asm__("FUN_L01_002b7c68");

void pool_bind_tile(float *in, int *tile_fixed_point_binding) {
    tile_fixed_point_binding[0] = truncate_float_to_s32((in[0] + pool_light[2]) * 1024.0f);
    tile_fixed_point_binding[1] = truncate_float_to_s32((in[1] + pool_light[3]) * 1024.0f);
    tile_fixed_point_binding[2] = truncate_float_to_s32((in[2] - 1.0f) * 1024.0f);
    ((short *)tile_fixed_point_binding)[6] = truncate_float_to_s32(pool_light[0] * 1024.0f);
    ((short *)tile_fixed_point_binding)[7] = truncate_float_to_s32(pool_light[0] * 1024.0f);
}

/* Ported from rac1-decomp src/overlays/shared/vuchain_002B8C00.c (func_L01_002B8C00) */
#include "sda.h"

/* Clears each record's three buffer groups and resets its scalar parameters. */

typedef struct VuChainSlot {
    char data[0x400];
    char blocks[6][0x40];
    char tail[0x18];
    char pad[0x28];
} VuChainSlot;

typedef struct VuChainRecord {
    char        pad0[0x20];
    int         f20, f24;
    float       f28, f2C, f30, f34, f38, f3C, f40;
    int         f44;
    char        pad48[8];
    VuChainSlot slots[3];
} VuChainRecord;

extern void memset(void *, s32, s32);

void pool_init_tiles(VuChainRecord *records, int count) __asm__("FUN_L01_002b7a48");

void pool_init_tiles(VuChainRecord *records, int count) {
    int i;
    for (i = 0; i < count; i++) {
        int j;
        for (j = 0; j < 3; j++) {
            memset(records[i].slots[j].data, 0, 0x400);
            memset(records[i].slots[j].blocks[0], 0, 0x40);
            memset(records[i].slots[j].blocks[1], 0, 0x40);
            memset(records[i].slots[j].blocks[2], 0, 0x40);
            memset(records[i].slots[j].blocks[4], 0, 0x40);
            memset(records[i].slots[j].blocks[3], 0, 0x40);
            memset(records[i].slots[j].blocks[5], 0, 0x40);
            memset(records[i].slots[j].tail, 0, 0x18);
        }
        records[i].f30 = 1.57079637f;
        records[i].f34 = -1.57079637f;
        records[i].f38 = 0.02f;
        records[i].f3C = -0.031f;
        records[i].f40 = 0.03f;
        records[i].f20 = 0;
        records[i].f24 = 0;
        records[i].f28 = -0.0016f;
        records[i].f2C = -0.0016f;
        records[i].f44 = 0;
    }
}
