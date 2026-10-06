#include "types.h"
struct RotatingPreviewMoby;
struct RotatingPreviewOwner {
    u8 pad00[0x44];
    struct RotatingPreviewMoby *preview_moby;
};
struct RotatingPreviewMoby {
    u8 pad00[0x10];
    f32 x;
    f32 y;
    f32 z;
    u8 pad1C[0x18];
    s16 flags;
    u8 pad36[0xA];
    f32 rotation_x;
    f32 rotation_y;
    u8 pad48[0x2C];
    void (*update)(struct RotatingPreviewMoby *);
    struct RotatingPreviewOwner **preview_vars;
};
struct PreviewCamera {
    u8 pad00[0x140];
    f32 x;
    f32 y;
    f32 z;
};
extern struct PreviewCamera preview_camera __asm__("D_00186F40");
extern struct RotatingPreviewMoby *create_menu_preview_moby(s32) __asm__("func_00225490");
extern void rotate_preview_moby_x(struct RotatingPreviewMoby *) __asm__("FUN_0021f120");
s32 create_rotating_preview_moby(struct RotatingPreviewOwner *owner) __asm__("FUN_0021ea48");

s32 create_rotating_preview_moby(struct RotatingPreviewOwner *owner) {
    struct RotatingPreviewMoby *moby = create_menu_preview_moby(0x46E);

    if (moby != 0) {
        owner->preview_moby = moby;
        moby->flags = 0;
        moby->x = preview_camera.x + 8.0f;
        moby->y = preview_camera.y + 0.5f;
        moby->z = preview_camera.z - 0.1f;
        moby->rotation_y = -1.9f;
        moby->update = rotate_preview_moby_x;
        *moby->preview_vars = owner;
    }
    return 0;
}

extern __typeof__(create_rotating_preview_moby) func_0021EA48
    __attribute__((alias("FUN_0021ea48")));
