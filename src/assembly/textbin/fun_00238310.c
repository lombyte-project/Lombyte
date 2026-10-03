#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00238310/FUN_00238310.s", FUN_00238310);
#else
#include "types.h"

extern s32 game_frame_counter __asm__("D_0015F438");
extern volatile s32 capture_glyph_coordinates[] __asm__("D_001E6018");
extern s32 capture_glyph_advances[] __asm__("D_001E6118");
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64) __asm__("func_001F5450");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 convert_float_to_integer(f32) __asm__("func_001FA6D0");
extern s32 find_valid_animation_frame_index(s32, s32) __asm__("func_001FF960");
extern u64 get_frame_texture(s32) __asm__("func_001FFA10");

void render_capture_scrolling_text(u8 *text, s32 start_x, s32 start_y, f32 scale) __asm__("FUN_00238310");

void render_capture_scrolling_text(u8 *text, s32 start_x, s32 start_y, f32 scale) {
    s64 texture;
    s32 x;
    s32 y;
    s32 glyph_index;
    s32 blink_next_glyph;
    s32 skip_draw;
    s32 long_blink_period;
    s32 short_blink_period;
    s32 blink_period;
    s32 blink_frame;
    s32 packed_coordinates;
    s32 texture_v;
    s32 texture_u;
    s32 width;
    s32 height;
    s32 advance_offset;
    u8 character;

    x = start_x;
    y = start_y;
    blink_next_glyph = 0;
    texture = get_frame_texture(find_valid_animation_frame_index(0xE935, 0));

    for (;;) {
        character = *text;
        if (character == 0) {
            break;
        }
        text++;
        glyph_index = (s32)character - 0x20;
        skip_draw = 0;

        if (glyph_index == 0x42) {
            blink_next_glyph = 1;
        }
        if (glyph_index == 0xEA) {
            x = start_x;
            y += 9;
        }

        if (glyph_index < 0x3B) {
            advance_offset = glyph_index * 4;
            {
                volatile s32 *glyph_entry;
                glyph_entry = capture_glyph_coordinates + glyph_index;
                if (*glyph_entry != -1 || glyph_index == 0) {
                    if (blink_next_glyph != 0) {
                        blink_next_glyph = 0;
                        long_blink_period = scale_game_frames(0x1E);
                        short_blink_period = scale_game_frames(0xA);
                        blink_period = long_blink_period + short_blink_period;
                        blink_frame = game_frame_counter % blink_period;
                        long_blink_period = scale_game_frames(0xA);
                        skip_draw = 1;
                        if (blink_frame >= long_blink_period) {
                            skip_draw = 0;
                        }
                    }

                    if (glyph_index != 0 && skip_draw == 0) {
                        packed_coordinates = *glyph_entry;
                        texture_v = (u32)packed_coordinates >> 0x14;
                        texture_u = (packed_coordinates & 0xFFFF) >> 4;

                        if (x + 9 > 0) {
                            if (x < 0x100) {
                                width = convert_float_to_integer(convert_integer_to_float(9) * scale);
                                height = convert_float_to_integer(convert_integer_to_float(9) * scale);
                                draw_textured_quad(
                                    x, y, width, height, texture_u, texture_v, 9, 9,
                                    (s64)0x80404040, texture);
                            }
                        }
                    }

                    x += convert_float_to_integer(convert_integer_to_float(*(s32 *)((u8 *)capture_glyph_advances + advance_offset)) * scale);
                }
            }
        }
    }
}

extern __typeof__(render_capture_scrolling_text) func_00238310 __attribute__((alias("FUN_00238310")));

#endif /* NON_MATCHING */
