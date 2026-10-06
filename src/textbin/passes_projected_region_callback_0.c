#include "types.h"
#include "asm.h"

#include "types.h"

#include "eetypes.h"

struct RegionPlayerState {
    u8 pad0[0x12E4];
    u8 region_mode;
    u8 pad12E5[0x208C - 0x12E5];
    s32 active_state;
};

extern struct RegionPlayerState player_state __asm__("D_0013F350");
extern s32 region_enabled[] __asm__("D_001A03B0");

union RegionVector {
    u128 region_center;
    f32 f[4];
};
extern s32 func_00208818(s32, s32, s32, s32, s32, s32);
extern f32 func_001F9B80(union RegionVector *, union RegionVector *);

s32 passes_projected_region_callback_0(s32 projected_x, s32 projected_y, f32 x, f32 y,
                                       f32 z) __asm__("FUN_00206bd8");

s32 passes_projected_region_callback_0(s32 projected_x, s32 projected_y, f32 x, f32 y, f32 z) {
    s32 special_state;

    if (func_00208818(projected_x, projected_y, 0x189, 0x16F, 0x87, 0xED)) {
        if (func_00208818(projected_x, projected_y, 0x75, 0x146, 0x18B, 0x1CC) &&
            func_00208818(projected_x, projected_y, 0x141, 0x130, 0xB0, 0x130)) {
            if (43.9f <= z) {
                return 1;
            }
            return 0;
        }
    } else if (func_00208818(projected_x, projected_y, 0x191, 0xCD, 0xD2, 0x13B)) {
        union RegionVector position;
        union RegionVector region_center;

        position.region_center = 0;
        region_center.region_center = 0;
        position.f[0] = x;
        position.f[1] = y;
        region_center.f[0] = 337.5f;
        region_center.f[1] = 250.0f;
        if (func_001F9B80(&position, &region_center) <= 7.0f) {
            return 1;
        }
        if (func_00208818(projected_x, projected_y, 0x142, 0x12A, 0x173, 0xFC) &&
            region_enabled[0]) {
            return 1;
        }
        return 0;
    } else {
        special_state = 0;
        if ((u32)(player_state.active_state - 0x11) < 2 || player_state.region_mode == 1) {
            special_state = 1;
        }
        if (special_state && func_00208818(projected_x, projected_y, 0x12B, 0xB8, 0x13A, 0xF2) &&
            func_00208818(projected_x, projected_y, 0x130, 0xED, 0x165, 0xCB) &&
            func_00208818(projected_x, projected_y, 0x161, 0xDC, 0x13B, 0xB0) &&
            func_00208818(projected_x, projected_y, 0x156, 0xA8, 0x119, 0xD1)) {
            return 1;
        }
    }
    return 0;
}
extern __typeof__(passes_projected_region_callback_0) func_00206BD8
    __attribute__((alias("FUN_00206bd8")));
