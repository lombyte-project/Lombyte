#include "types.h"

#include "rnc/rendering/fs_aa_buffer.h"
#include "rnc/rendering/screen.h"
#include "rnc/rendering/view.h"
#include "rnc/math/fast_math.h"

void init_view_context(void) __asm__("FUN_001f2c60");

void init_view_context(void) {
    struct FsAaBuf *fs_aa_buf = &fs_aa_buffer;
    struct View    *view = &view_context;
    s32             hw;
    s32             hh;

    hw = fs_aa_buf->display_width >> 1;
    hh = fs_aa_buf->display_height >> 1;
    screen_extent.width = fs_aa_buf->display_width;
    screen_extent.height = fs_aa_buf->display_height;
    screen_extent.half_width = hw;
    screen_extent.half_height = hh;
    screen_extent.left = (0x800 - hw) << 4;
    screen_extent.top = (0x800 - hh) << 4;
    screen_extent.right = (hw + 0x800) << 4;
    screen_extent.bottom = (hh + 0x800) << 4;
    view->near_clip = 32.0f;
    view->far_clip = 745472.0f;
    view->fov.f[0] = 0.63f;
    view->half_width = convert_integer_to_float(fs_aa_buf->display_width) * 0.5f;
    view->half_height = convert_integer_to_float(fs_aa_buf->display_height) * 0.5f;
    view->scr_x = view->half_width * 4.0f;
    view->scr_y = view->half_height * 4.0f;
    view->fog_far_dist = 524288.0f;
    view->fog_near_int = 255.0f;
    view->fog_near_dist = 0;
    view->fog_far_int = 0;
}

extern __typeof__(init_view_context) func_001F2C60 __attribute__((alias("FUN_001f2c60")));
