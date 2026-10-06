#include "types.h"
#include "asm.h"

#include "types.h"

extern s32 vendor_flash_timers[] __asm__("D_001E6620");
extern s32 vendor_scroll_timers[] __asm__("D_001E6640");
extern void vu1_add_g_sregister(s32, u64) __asm__("func_00233980");
extern s32 random_integer_below(s32) __asm__("func_00213260");
extern s32 SubtractIntegerWithClamp(s32);
extern s64 get_effect_texture(s32) __asm__("func_001F44B8");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern void append_subpixel_textured_screen_quad(f32, f32, f32, f32, s32, s32, s32, s32, u64,
                                                 s64) __asm__("func_001F55D8");

void render_vendor_capture_texture_overlays_pass(s32 pass_index, f32 capture_width,
                                                 f32 capture_height) __asm__("FUN_00239328");

void render_vendor_capture_texture_overlays_pass(s32 pass_index, f32 capture_width,
                                                 f32 capture_height) {
    s32 flash_timer;
    s32 scroll_opacity;
    s32 flash_opacity;
    f32 random_u;
    f32 random_v;
    f32 zero_offset;
    f32 scroll_offset;
    f32 overlay_height;
    s32 overlay_width;

    vu1_add_g_sregister(0x47, 0x32003);
    if (vendor_flash_timers[pass_index] != 0 || pass_index == 0) {
        if (vendor_flash_timers[pass_index] != 0) {
            vendor_flash_timers[pass_index] += 2;
        }
        flash_timer = vendor_flash_timers[pass_index];
        if (pass_index == 0) {
            /* The floor changes this frame's opacity, not the stored timer. */
            if (flash_timer < 0x18) {
                flash_timer = 0x18;
            }
        }
        random_u = random_integer_below(200);
        random_v = random_integer_below(200);
        zero_offset = 0.0f;
        flash_opacity = 0x80 - SubtractIntegerWithClamp(flash_timer - 0x80);
        vu1_add_g_sregister(8, 0);
        flash_opacity *= 2;
        if (flash_opacity > 0x80)
            flash_opacity = 0x80;
        vu1_add_g_sregister(0x42, ((u64)flash_opacity << 32) | 0x68);
        append_subpixel_textured_screen_quad(0.0f, 0.0f, capture_width, capture_height,
                                             random_u + zero_offset, random_v + zero_offset,
                                             capture_width + random_u, capture_height + random_v,
                                             0x808080, get_effect_texture(0x1A));
        if (vendor_flash_timers[pass_index] >= 0x100) {
            vendor_flash_timers[pass_index] = 0;
        }
    }
    if (vendor_flash_timers[pass_index] == 0 && random_integer_below(700) == 0) {
        vendor_flash_timers[pass_index] = 2;
    }
    vu1_add_g_sregister(8, 0);
    vu1_add_g_sregister(0x42, 0x8000000044ULL);
    if (pass_index > 0) {
        if (vendor_scroll_timers[pass_index] != 0) {
            vendor_scroll_timers[pass_index] += 2;
            overlay_width = capture_width;
            scroll_opacity =
                0x100 - SubtractIntegerWithClamp(vendor_scroll_timers[pass_index] - 0x100);
            if (scroll_opacity > 0x50) {
                scroll_opacity = 0x50;
            }
            scroll_offset =
                -(convert_integer_to_float(0x200 - vendor_scroll_timers[pass_index]) * 0.03125f);
            overlay_height = capture_height + 16.0f;
            append_subpixel_textured_screen_quad(0.0f, scroll_offset, capture_width, overlay_height,
                                                 0, 0, overlay_width, (s32)(overlay_height * 1.5f),
                                                 (scroll_opacity << 24) | 0x505050,
                                                 get_effect_texture(0x1C));
            if (vendor_scroll_timers[pass_index] >= 0x200) {
                vendor_scroll_timers[pass_index] = 0;
            }
        } else if (random_integer_below(360) == 0) {
            vendor_scroll_timers[pass_index] = 2;
        }
    }
    if (pass_index == 6) {
        /* The known capture coordinator supplies only passes zero through five. */
        append_subpixel_textured_screen_quad(0.0f, 0.0f, capture_width, capture_height, 0, 0, 0x40,
                                             0x40, 0x80808080, get_effect_texture(0x19));
    }
}
extern __typeof__(render_vendor_capture_texture_overlays_pass) func_00239328
    __attribute__((alias("FUN_00239328")));
