#include "types.h"

struct CameraProbePosition {
    f32 x;
    f32 y;
    f32 z;
};

extern s32 D_00161198;
extern s32 D_0016119C;
extern s32 D_001611A0;
extern f32 D_001611AC;
extern f32 AbsoluteFloat(f32) __asm__("func_001F99C0");
extern void FUN_001f9a00(s32);
extern f32 FUN_001f9b80(struct CameraProbePosition *, s32 *);
extern s32 sample_surface_height_map(f32 *, f32, f32, f32) __asm__("func_00239F58");

f32 resolve_camera_surface_height(struct CameraProbePosition *position, s32 optional_output) __asm__("FUN_002135f0");

f32 resolve_camera_surface_height(struct CameraProbePosition *position, s32 optional_output) {
    f32 surface_height;
    if (D_00161198 != 0) {
        surface_height = position->z;
        if (sample_surface_height_map(&surface_height, position->x, position->y, surface_height) != 0) {
            return surface_height;
        }
    }
    if (D_0016119C != 0) {
        if (AbsoluteFloat(position->z - *(f32 *)0x1611A8) < 0.5f) {
            if (FUN_001f9b80(position, &D_001611A0) < D_001611AC) {
                if (optional_output != 0) {
                    FUN_001f9a00(optional_output);
                }
                return *(f32 *)0x1611A8;
            }
        }
    }
    if (optional_output != 0) {
        FUN_001f9a00(optional_output);
    }
    return position->z;
}

extern __typeof__(resolve_camera_surface_height) func_002135F0 __attribute__((alias("FUN_002135f0")));
