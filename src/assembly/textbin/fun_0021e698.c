#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021e698/FUN_0021e698.s", FUN_0021e698);
#else
#include "types.h"

struct PreviewPosition {
    f32 x;
    f32 y;
    f32 z;
};

struct ItemPreviewPlacement {
    f32 alternate_x;
    f32 normal_x;
    f32 y;
    f32 z;
    u8 pad10[8]; /* Per-item record stride is 0x20. */
    f32 side_offset;
    f32 forward_offset;
};

struct ItemPreviewBinding {
    u8 pad0[0x30];
    s32 flags;
    u8 pad34[4];
    f32 rotation_angle;
};

struct ItemPreviewVars {
    struct ItemPreviewBinding *owner;
    u8 pad4[8];
    s32 item_index;
};

struct ItemPreviewMoby {
    u8 pad0[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 pad1C[0x2C];
    f32 rotation_z;
    u8 pad4C[0x2C];
    struct ItemPreviewVars *preview_vars;
};

struct PreviewCamera {
    u8 pad0[0x140];
    struct PreviewPosition position;
};

extern struct PreviewCamera preview_camera __asm__("D_00186F40");
extern struct ItemPreviewPlacement preview_placements[] __asm__("D_001E0408");
extern f32 fast_cos(f32) __asm__("func_001F9DC8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");

void update_item_preview_transform(struct ItemPreviewMoby *moby) __asm__("FUN_0021e698");

/* Position the selected item relative to the preview camera. Owner flag bit 0
   selects alternate_x; otherwise use normal_x. Rotate the side and forward
   offsets in the X/Y plane after adding the unrotated placement position. */
void update_item_preview_transform(struct ItemPreviewMoby *moby) {
    struct ItemPreviewBinding *preview = moby->preview_vars->owner;
    s32 item_index = moby->preview_vars->item_index;
    f32 side_offset;
    f32 forward_offset;
    f32 cosine;
    f32 sine;
    f32 negated_forward_offset;

    moby->rotation_z = preview->rotation_angle;
    moby->x = (&preview_camera.position)->x + ((preview->flags & 1) ? preview_placements[item_index].alternate_x : preview_placements[item_index].normal_x);
    moby->y = (&preview_camera.position)->y + preview_placements[item_index].y;
    moby->z = (&preview_camera.position)->z + preview_placements[item_index].z;
    forward_offset = preview_placements[item_index].forward_offset;
    side_offset = preview_placements[item_index].side_offset;
    cosine = fast_cos(moby->rotation_z);
    negated_forward_offset = -forward_offset;
    sine = fast_sin(moby->rotation_z);
    moby->x += negated_forward_offset * sine + side_offset * cosine;
    moby->y += forward_offset * cosine + side_offset * sine;
}
#endif /* NON_MATCHING */
