#include "types.h"
struct RotatingPreviewMoby {
    u8 pad_0[0x40];
    f32 rotation_x;
};
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
void rotate_preview_moby_x(struct RotatingPreviewMoby *moby) __asm__("FUN_0021f120");

void rotate_preview_moby_x(struct RotatingPreviewMoby *moby) {
    moby->rotation_x = fast_add_rotations(moby->rotation_x, 0.02f);
}
