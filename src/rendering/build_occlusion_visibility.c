#include "types.h"

typedef struct {
    u8 pad_0[0x140];
    f32 x;
    f32 y;
    f32 z;
} Camera;

extern Camera D_00186F40;
extern u8 D_00193FC0[];
extern u8 D_00194040[];
extern f32 *D_0015F644;
extern s32 D_0015F648;
extern s32 D_0015F64C;
extern u8 *D_0015F650;
extern s32 D_0018C32C[];

extern u8 *parse_occlusion_grid(s32, s32, s32) __asm__("FUN_001f2690");
extern u8 *get_occlusion_grid_from_pair(s32, s32, s32, s32, s32, s32, f32) __asm__("FUN_001f2768");
extern void FUN_001f9810(u8 *, s32);
extern void FUN_001f98d0(u8 *, u8 *, s32);
extern void FUN_001f98f8(u8 *, u8 *, u8 *, s32);
extern f32 func_001FA6C0(s32);
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern void FillTransferWords(u8 *, s32, s32);

void build_occlusion_visibility(void) __asm__("FUN_001f2820");

void build_occlusion_visibility(void) {
    f32 scale = 0.25f;
    s32 x, y, z;
    u8 *vis;
    u8 *gx, *gy, *gz;
    f32 *p;
    s32 bx, by, bz;

    x = truncate_float_to_s32(D_00186F40.x * scale);
    y = truncate_float_to_s32(D_00186F40.y * scale);
    z = truncate_float_to_s32(D_00186F40.z * scale);
    vis = parse_occlusion_grid(x, y, z);
    if (vis != 0) {
        D_0015F64C = 0;
        FUN_001f98d0(D_00193FC0, vis, 0x80);
        D_0015F650 = vis;
    } else {
        D_0015F64C = 1;
        if (D_0015F648 == 0) {
            gx = get_occlusion_grid_from_pair(x - 1, y, z, x + 1, y, z,
                                              D_00186F40.x * scale - func_001FA6C0(x));
            gy = get_occlusion_grid_from_pair(x, y - 1, z, x, y + 1, z,
                                              D_00186F40.y * scale - func_001FA6C0(y));
            gz = get_occlusion_grid_from_pair(x, y, z - 1, x, y, z + 1,
                                              D_00186F40.z * scale - func_001FA6C0(z));
            if (gx != 0 || gy != 0 || gz != 0) {
                FUN_001f9810(D_00194040, 0x80);
                if (gx != 0) {
                    FUN_001f98f8(D_00194040, D_00194040, gx, 0x80);
                }
                if (gy != 0) {
                    FUN_001f98f8(D_00194040, D_00194040, gy, 0x80);
                }
                if (gz != 0) {
                    FUN_001f98f8(D_00194040, D_00194040, gz, 0x80);
                }
                vis = D_00194040;
                D_0015F650 = vis;
                FUN_001f98d0(D_00193FC0, vis, 0x80);
            }
        }
        if (vis == 0) {
            switch (D_0015F648) {
            case 0:
                if (D_0018C32C[0] == 0 && D_0015F650 != 0) {
                    FUN_001f98d0(D_00193FC0, D_0015F650, 0x80);
                } else {
                    FillTransferWords(D_00193FC0, -1, 0x80);
                }
                break;
            case 1:
                FillTransferWords(D_00193FC0, -1, 0x80);
                break;
            case 2:
                p = D_0015F644;
                if (p != 0) {
                    bx = 0.0f < D_00186F40.x - p[0];
                    by = 0.0f < D_00186F40.y - p[1];
                    bz = 0.0f < D_00186F40.z - p[2];
                    FUN_001f98d0(D_00193FC0, (u8 *)p + ((bz + by * 2 + bx * 4) * 0x80 + 0x10),
                                 0x80);
                } else if (D_0018C32C[0] == 0 && D_0015F650 != 0) {
                    FUN_001f98d0(D_00193FC0, D_0015F650, 0x80);
                } else {
                    FillTransferWords(D_00193FC0, -1, 0x80);
                }
                break;
            }
        }
    }
    D_00193FC0[0x7F] |= 0x80;
}

extern __typeof__(build_occlusion_visibility) func_001F2820 __attribute__((alias("FUN_001f2820")));
