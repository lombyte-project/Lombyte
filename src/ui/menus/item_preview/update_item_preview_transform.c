#include "types.h"
#include "asm.h"

#include "types.h"
#include "rnc/ui/menus/item_preview/item_preview_placement.h"

struct PreviewPosition {
    f32 x;
    f32 y;
    f32 z;
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
    volatile f32 x;
    f32 y;
    f32 z;
    u8 pad1C[0x2C];
    f32 rotation_z;
    u8 pad4C[0x2C];
    struct ItemPreviewVars *preview_vars;
};

struct PreviewCamera {
    u8 pad0[0x140];
    f32 x;
    f32 y;
    f32 z;
};

extern struct PreviewCamera preview_camera __asm__("D_00186F40");
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
    s32 flags = preview->flags;
    f32 y_offset;
    struct PreviewCamera *camera;

    moby->rotation_z = preview->rotation_angle;
    moby->x = preview_camera.x + ((flags & 1) ? preview_placements[item_index].alternate_x
                                              : preview_placements[item_index].normal_x);
    camera = &preview_camera;
    y_offset = preview_placements[item_index].y;
    moby->y = camera->y + y_offset;
    moby->z = camera->z + preview_placements[item_index].z;
    forward_offset = preview_placements[item_index].forward_offset;
    side_offset = preview_placements[item_index].side_offset;
    cosine = fast_cos(moby->rotation_z);
    negated_forward_offset = -forward_offset;
    sine = fast_sin(moby->rotation_z);
    moby->x += negated_forward_offset * sine + side_offset * cosine;
    moby->y += forward_offset * cosine + side_offset * sine;
}
