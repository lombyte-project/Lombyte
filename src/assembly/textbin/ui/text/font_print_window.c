#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/text/font_print_window/FUN_001f7090.s",
            FUN_001f7090);
#else
#include "types.h"
#include "rnc/ui/text/text_region.h"

typedef struct TextRegion FontWindow;

struct Glyph {
    u8 u;
    u8 v;
    s8 top;
    s8 advance;
};

extern s32 font_window_active __asm__("D_0015F4A0");
extern s32 font_color_codes_enabled __asm__("D_0015F49C");
#include "rnc/ui/text/font_palette.h"
#include "rnc/rendering/screen.h"
extern void vu1_set_scissor(s32, s32, s32, s32) __asm__("func_00233A40");
extern s32 measure_text_width(u8 *, s32, struct Glyph *) __asm__("func_001F6200");
extern void font_print(s32, s32, u64, u8 *, s32, s64, struct Glyph *) __asm__("func_001F62B0");
extern void process_bgm_display_text_event(s32, u8 *, s32, s64, struct Glyph *, f32, f32,
                                           f32) __asm__("func_001F6638");

void font_print_window(FontWindow *window, u64 color, u8 *text, s32 character_limit, s64 texture,
                       struct Glyph *glyphs) __asm__("FUN_001f7090");

void font_print_window(FontWindow *window, u64 color, u8 *text, s32 character_limit, s64 texture,
                       struct Glyph *glyphs) {
    s16 line_starts[32];
    s16 line_ends[32];
    s16 line_colors[32];
    s32 wrap_width;
    volatile s32 initial_wrap_width;
    s32 using_initial_width;
    s32 initial_line_count;
    s32 last_line_width;
    s32 line_count;
    s32 next_line_count;
    s32 current_color;
    s32 position;
    s32 break_position;
    s32 line_width;
    s32 advance;
    s32 right_width;
    s32 left_width;
    s32 balance_divisor;
    s32 line_index;
    s32 draw_y;
    s32 line_length;
    s32 width;
    f32 subpixel_x;
    FontWindow *draw_window;

    vu1_set_scissor(window->left, window->right - 1, window->top, window->bottom - 1);
    font_window_active = 1;
    if ((window->flags ^ TEXT_REGION_CENTER_HORIZONTALLY) & TEXT_REGION_CENTER_HORIZONTALLY) {
        wrap_width = window->right - window->anchor_x;
    } else {
        left_width = window->anchor_x;
        right_width = window->right - left_width;
        left_width -= window->left;
        if (left_width > right_width) {
            left_width = right_width;
        }
        wrap_width = left_width * 2;
    }
    current_color = 0;
    initial_wrap_width = wrap_width;
    using_initial_width = 0;
    initial_line_count = 0;
    last_line_width = 0;
    balance_divisor = 3;
retry:
    line_count = 0;
    position = 0;
    while (position != character_limit && text[position] != 0) {
        line_starts[line_count] = position;
        line_colors[line_count] = current_color;
        break_position = position;
        line_width = 0;
        if (wrap_width > 0) {
            do {
                if (text[position] == ' ' || text[position] < 0x10) {
                    break_position = position;
                }
                if (font_color_codes_enabled != 0 &&
                    (text[position] >= 8 && text[position] < 0x10)) {
                    current_color = text[position] - 8;
                }
                if (text[position] < 2) {
                    break;
                }
                advance = glyphs[text[position++]].advance;
                if (advance != 0) {
                    line_width += advance;
                }
            } while (line_width < wrap_width);
        }
        line_ends[line_count] = break_position;
        if (line_ends[line_count] == line_starts[line_count]) {
            line_ends[line_count] = position;
        }
        position = line_ends[line_count];
        if (text[position] == ' ' || text[position] < 0x10) {
            line_ends[line_count]--;
        }
        line_count++;
        if (text[position] == 0) {
                goto terminal_line;
        }
        position++;
    }
after_scan:
    next_line_count = line_count < 2;
    if (using_initial_width) {
        goto layout;
    }
    initial_line_count = initial_line_count ? initial_line_count : line_count;
    if (next_line_count) {
        goto layout;
    }
    if (initial_line_count < line_count) {
        wrap_width = initial_wrap_width;
        using_initial_width = 1;
        goto retry;
    }
    goto compare_width;

terminal_line:
    /* Retail saves this width before testing whether to rebalance. */
    last_line_width = line_width;
    goto after_scan;

compare_width:
    if (last_line_width < wrap_width / balance_divisor) {
        wrap_width -= 0x10;
        goto retry;
    }

layout:
    draw_window = window;
    line_length = line_count * draw_window->line_advance;
    draw_window->measured_width = 0;
    draw_y = draw_window->anchor_y;
    draw_window->rendered_height = line_length;
    if (draw_window->flags & TEXT_REGION_CENTER_VERTICALLY) {
        draw_y -= line_length >> 1;
    }
    for (line_index = 0; line_index < line_count;
         line_index++, draw_y += draw_window->line_advance) {
        if (draw_y + draw_window->line_advance < draw_window->top) {
            continue;
        }
        if (draw_window->bottom < draw_y) {
            continue;
        }
        line_length = line_ends[line_index] - line_starts[line_index] + 1;
        width = measure_text_width(text + line_starts[line_index], line_length, glyphs);
        if (draw_window->measured_width < width) {
            draw_window->measured_width = width;
        }
        if (draw_window->flags & TEXT_REGION_MEASURE_ONLY) {
            continue;
        }
        font_palette_colors[0] = color;
        if (draw_window->flags & TEXT_REGION_USE_SUBPIXEL_RENDERER) {
            if (draw_window->flags & TEXT_REGION_CENTER_HORIZONTALLY) {
                subpixel_x = (f32)(draw_window->anchor_x - (width >> 1)) +
                             (f32)draw_window->subpixel_x_sixteenths * 0.0625f;
            } else {
                subpixel_x =
                    (f32)draw_window->anchor_x + (f32)draw_window->subpixel_x_sixteenths * 0.0625f;
            }
            process_bgm_display_text_event(
                font_palette_colors[line_colors[line_index]], text + line_starts[line_index],
                line_length, texture, glyphs, subpixel_x,
                (f32)draw_y + (f32)draw_window->subpixel_y_sixteenths * 0.0625f, 1.0f);
        } else if (draw_window->flags & TEXT_REGION_CENTER_HORIZONTALLY) {
            font_print(draw_window->anchor_x - (width >> 1), draw_y,
                       font_palette_colors[line_colors[line_index]], text + line_starts[line_index],
                       line_length, texture, glyphs);
        } else {
            font_print(draw_window->anchor_x, draw_y, font_palette_colors[line_colors[line_index]],
                       text + line_starts[line_index], line_length, texture, glyphs);
        }
    }
    font_window_active = 0;
    left_width = screen_extent.width;
    vu1_set_scissor(0, left_width - 1, 0, screen_extent.height - 1);
}

extern __typeof__(font_print_window) func_001F7090 __attribute__((alias("FUN_001f7090")));
#endif /* NON_MATCHING */
