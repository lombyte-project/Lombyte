#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00238310/FUN_00238310.s", FUN_00238310);
#else
#include "types.h"

extern s32 game_frame_counter __asm__("D_0015F438");
extern volatile s32 capture_glyph_coordinates[] __asm__("D_001E6018");
extern s32 capture_glyph_advances[] __asm__("D_001E6118");
extern void draw_textured_quad(s32, s32, s32, s32, s32, s32, s32, s32, s64,
                               s64) __asm__("func_001F5450");
extern s32 scale_game_frames(s32) __asm__("func_001F96F8");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 convert_float_to_integer(f32) __asm__("func_001FA6D0");
extern s32 get_icon_frame(s32, s32) __asm__("func_001FF960");
extern u64 get_frame_texture(s32) __asm__("func_001FFA10");

void render_capture_scrolling_text(char *text, s32 start_x, s32 start_y,
                                   f32 scale) __asm__("FUN_00238310");

void render_capture_scrolling_text(char *text, s32 start_x, s32 start_y, f32 scale) {
    s64 texture;
    s32 cursor_x;
    s32 cursor_y;
    s32 glyph_index;
    s32 blink_next_glyph;
    s32 skip_draw;
    s32 long_blink_period;
    s32 short_blink_period;
    s32 blink_period;
    s32 blink_remainder;
    char *cursor_text;
    s32 packed_coordinates;
    s32 texture_v;
    s32 texture_u;
    s32 glyph_width;
    s32 glyph_height;
    u8 character;

    blink_next_glyph = 0;
    cursor_text = text;
    texture = get_frame_texture(get_icon_frame(0xE935, 0));
    cursor_x = start_x;
    cursor_y = start_y;

    for (;;) {
        skip_draw = 0;
        character = (u8)*cursor_text;
        if (character == 0) {
            return;
        }
        cursor_text++;
        glyph_index = (s32)character - 0x20;

        if (glyph_index == 0x42) {
            blink_next_glyph = 1;
        }
        /* Retail retains this branch even though an unsigned byte minus 0x20 cannot reach 0xEA. */
        if (glyph_index == 0xEA) {
            cursor_x = start_x;
            cursor_y += 9;
        }

        if (glyph_index < 0x3B) {
            {
                volatile s32 *glyph_entry;
                glyph_entry = capture_glyph_coordinates + glyph_index;
                if (*glyph_entry != -1 || glyph_index == 0) {
                    if (blink_next_glyph != 0) {
                        skip_draw = 0;
                        blink_next_glyph = 0;
                        long_blink_period = scale_game_frames(0x1E);
                        short_blink_period = scale_game_frames(0xA);
                        blink_period = long_blink_period + short_blink_period;
                        blink_remainder = game_frame_counter % blink_period;
                        long_blink_period = scale_game_frames(0xA);
                        if (blink_remainder < long_blink_period) {
                            skip_draw = 1;
                        }
                    }

                    if (glyph_index != 0 && skip_draw == 0) {
                        packed_coordinates = *glyph_entry;
                        texture_v = (u32)packed_coordinates >> 0x14;
                        texture_u = (packed_coordinates & 0xFFFF) >> 4;

                        if (cursor_x + 9 > 0) {
                            if (cursor_x < 0x100) {
                                glyph_width =
                                    convert_float_to_integer(convert_integer_to_float(9) * scale);
                                glyph_height =
                                    convert_float_to_integer(convert_integer_to_float(9) * scale);
                                draw_textured_quad(cursor_x, cursor_y, glyph_width, glyph_height,
                                                   texture_u, texture_v, 9, 9, (s64)0x80404040,
                                                   texture);
                            }
                        }
                    }

                    cursor_x += convert_float_to_integer(
                        convert_integer_to_float(capture_glyph_advances[glyph_index]) * scale);
                }
            }
        }
    }
}

extern __typeof__(render_capture_scrolling_text) func_00238310
    __attribute__((alias("FUN_00238310")));

#endif /* NON_MATCHING */
