#include "types.h"

#include "rnc/rendering/fs_aa_buffer.h"
#include "rnc/rendering/screen.h"
#include "rnc/rendering/view.h"
extern f32 func_001FA6C0(s32);

void init_view_context(void) __asm__("FUN_001f2c60");

void init_view_context(void) {
    struct FsAaBuf *d = &fs_aa_buffer;
    struct View *v = &view_context;
    s32 hw;
    s32 hh;

    hw = d->display_width >> 1;
    hh = d->display_height >> 1;
    screen_extent.width = d->display_width;
    screen_extent.height = d->display_height;
    screen_extent.half_width = hw;
    screen_extent.half_height = hh;
    screen_extent.left = (0x800 - hw) << 4;
    screen_extent.top = (0x800 - hh) << 4;
    screen_extent.right = (hw + 0x800) << 4;
    screen_extent.bottom = (hh + 0x800) << 4;
    v->near_clip = 32.0f;
    v->far_clip = 745472.0f;
    v->fov.f[0] = 0.63f;
    v->half_width = func_001FA6C0(d->display_width) * 0.5f;
    v->half_height = func_001FA6C0(d->display_height) * 0.5f;
    v->scr_x = v->half_width * 4.0f;
    v->scr_y = v->half_height * 4.0f;
    v->fog_far_dist = 524288.0f;
    v->fog_near_int = 255.0f;
    v->fog_near_dist = 0;
    v->fog_far_int = 0;
}

extern __typeof__(init_view_context) func_001F2C60 __attribute__((alias("FUN_001f2c60")));
