/* HUD: draws a segment meter. */
#include "types.h"
#include "asm.h"
#include "rnc/globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "sda.h"
#include "rnc/ui/hud/get_icon_frame.h"
#include "rnc/ui/hud/sprites/draw_hud_sprite.h"
#include "rnc/ui/hud/sprites/draw_hud_sprite_flipped.h"

extern s32  frame_size __asm__("D_0013E504") NOT_SDA;
extern s32  D_L06_0015F8E8 __attribute__((sda));
extern f32  D_L06_0015F8F0 __attribute__((sda));
extern f32  D_L06_0015F8F4 __attribute__((sda));
extern s32  fast_tween_color(s32, s32, f32) __asm__("FUN_001fa6e0");
extern void append_screen_rect_packet(s32, s32, s32, s32, u64, s32) __asm__("FUN_00200e08");

s32 draw_hud_segment_meter(u8 *p) __asm__("FUN_L06_00249738");

s32 draw_hud_segment_meter(u8 *p) {
    s32 y;
    s32 ntsc_y;
    s32 level;
    s32 segments = 3;
    s32 x, start_x, inner, i, color, remaining, tile_count, k;
    s32 center_x = D_L06_0015F8E8;
    f32 alpha, scaled_alpha, fill, step, limit;

    /* Fakematch: computing this row before the default one gives retail's order. */
    ntsc_y = frame_size - 50;
    y = frame_size - 42;
    if (!pal_mode)
        y = ntsc_y;
    level = current_level_index;
    if (level == 7 || level == 13)
        segments = 6;
    if (level == 18)
        segments = 7;
    if (p[0x70] == 0)
        return *(s32 *)(p + 0x58);
    {
        if (level == 13) {
            y = 18;
            if (pal_mode != 0)
                y = 10;
        }
        alpha = convert_integer_to_float(p[0x70]) * 0.125f;
        if (alpha > 1.0f)
            alpha = 1.0f;
        else if (alpha < 0.0f)
            alpha = 0.0f;
        remaining = segments - 1;
        scaled_alpha = alpha * 128.0f;
        i = truncate_float_to_s32(scaled_alpha);
        inner = truncate_float_to_s32(scaled_alpha);
        start_x = center_x - segments * 16;
        x = start_x;
        for (k = 0; k < segments; k++) {
            draw_hud_sprite_flipped(get_icon_frame(0x7562, 3), x, y + 6, 32, 18, i);
            x += 32;
        }
        x = 0;
        color = 0x800000ff;
        if (*(s32 *)(p + 0x74) != 0) {
            fill = (f32) * (s32 *)(p + 0x74) / convert_integer_to_float(*(s32 *)(p + 8));
            step = 1.0f / convert_integer_to_float(segments);
            limit = remaining * step;
            if (limit < fill) {
                fill -= limit;
                fill *= D_L06_0015F8F0;
                fill += limit;
            } else if (fill < step) {
                fill -= step;
                fill *= D_L06_0015F8F4;
                fill += step;
            }
            x = truncate_float_to_s32((f32)(segments << 5) * fill);
            if (0.5f < fill) {
                color = fast_tween_color(0x8000ffff, 0x8000ff00, (fill + fill) - 1.0f);
            } else {
                color = fast_tween_color(0x800000ff, 0x8000ffff, fill + fill);
            }
        }
        append_screen_rect_packet(start_x, y + 8, start_x + x, y + 26, color, 0);
        x = start_x;
        segments = x - 40;
        if (remaining > 0) {
            tile_count = remaining;
            do {
                draw_hud_sprite(get_icon_frame(0x7562, 1), x, y, 32, 32, inner);
                x += 32;
                tile_count--;
            } while (tile_count != 0);
        }
        draw_hud_sprite(get_icon_frame(0x7562, 2), x, y, 32, 32, inner);
        draw_hud_sprite(get_icon_frame(0x7562, 4), segments, y - 8, 48, 48, i);
        draw_hud_sprite(get_icon_frame(0x7562, 0), segments, y - 8, 48, 48, i);
    }
    return *(s32 *)(p + 0x58);
}

s32 D_L07_0015F8E8 = 0x114;
f32 D_L07_0015F8F0 = 0.7f;
f32 D_L07_0015F8F4 = 0.7f;
s32 D_L13_0015F8E8 = 0x114;
f32 D_L13_0015F8F0 = 0.7f;
f32 D_L13_0015F8F4 = 0.7f;
s32 D_L18_0015F8E8 = 0x114;
f32 D_L18_0015F8F0 = 0.7f;
f32 D_L18_0015F8F4 = 0.7f;
