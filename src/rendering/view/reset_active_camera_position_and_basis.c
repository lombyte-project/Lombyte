#include "types.h"
#include "qzero.h"
#include "rnc/ui/menus/menu_globals.h"
#include "rnc/rendering/view/reset_active_camera_position_and_basis_globals.h"

extern s32 D_001872B0;
void       reset_active_camera_position_and_basis(void) __asm__("FUN_00218d10");

void reset_active_camera_position_and_basis(void) {
    preview_camera.position.f[0] = 256.0f;
    preview_camera.position.f[2] = 64.0f;
    preview_camera.position.f[1] = 256.0f;
    qzero(&preview_camera.forward);
    qzero(&active_camera_basis_column1_x);
    qzero(&D_001872B0);
    preview_camera.forward.f[0] = 1.0f;
    preview_camera.right.f[1] = 1.0f;
    preview_camera.up.f[2] = 1.0f;
    preview_camera.up.f[3] = 1.0f;
}

extern __typeof__(reset_active_camera_position_and_basis) func_00218D10
    __attribute__((alias("FUN_00218d10")));
