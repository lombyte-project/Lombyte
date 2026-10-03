#include "types.h"
struct ItemPreviewRotation {
    u8 pad_0[0x48];
    f32 rotation_z;
};
extern f32 fast_add_rotations(f32, f32) __asm__("func_001FA580");
void rotate_item_preview_moby(struct ItemPreviewRotation *moby) __asm__("FUN_0021e1f8");

void rotate_item_preview_moby(struct ItemPreviewRotation *moby) {
    moby->rotation_z = fast_add_rotations(moby->rotation_z, 0.01f);
}
