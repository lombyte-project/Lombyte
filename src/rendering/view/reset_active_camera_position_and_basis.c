#include "types.h"
#include "qzero.h"
struct S {
    u8 pad_0[0x140];
    float f140, f144, f148;
    u8 pad_14C[0x204];
    float f350;
    u8 pad_354[0x10];
    float f364;
    u8 pad_368[0x10];
    float f378, f37C;
};
extern struct S preview_camera __asm__("D_00186F40");
#include "rnc/rendering/view/reset_active_camera_position_and_basis_globals.h"
extern s32 D_001872B0;
void reset_active_camera_position_and_basis(void) __asm__("FUN_00218d10");

void reset_active_camera_position_and_basis(void) {
    preview_camera.f140 = 256.0f;
    preview_camera.f148 = 64.0f;
    preview_camera.f144 = 256.0f;
    qzero(&preview_camera.f350);
    qzero(&active_camera_basis_column1_x);
    qzero(&D_001872B0);
    preview_camera.f350 = 1.0f;
    preview_camera.f364 = 1.0f;
    preview_camera.f378 = 1.0f;
    preview_camera.f37C = 1.0f;
}

extern __typeof__(reset_active_camera_position_and_basis) func_00218D10 __attribute__((alias("FUN_00218d10")));
