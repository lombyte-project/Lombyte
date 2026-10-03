#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/text/font_print_window/FUN_001f7090.s", FUN_001f7090);
#else
#include "types.h"

struct Glyph {
    u8 u;
    u8 v;
    s8 top;
    s8 advance;
};

struct FontWindow {
    s16 top;
    s16 bottom;
    s16 left;
    s16 right;
    s16 x;
    s16 y;
    s16 text_width;
    s16 text_height;
    s16 line_height;
    u16 flags;
    s16 offset_x;
    s16 offset_y;
};

struct ScreenOfs { s32 width; s32 height; };

extern s32 font_window_active __asm__("D_0015F4A0");
extern s32 font_color_codes_enabled[] __asm__("D_0015F49C");
extern s32 font_palette_colors[] __asm__("D_0018CAF8");
extern struct ScreenOfs D_0013E500;
extern void vu1_set_scissor(s32, s32, s32, s32) __asm__("func_00233A40");
extern s32 measure_text_width(u8 *, s32, struct Glyph *) __asm__("func_001F6200");
extern void font_print(s32, s32, u64, u8 *, s32, s64, struct Glyph *) __asm__("func_001F62B0");
extern void func_001F6638(s32, u8 *, s32, s64, struct Glyph *, f32, f32, f32);

void font_print_window(struct FontWindow *window, u64 color, u8 *text, s32 character_limit, s64 texture, struct Glyph *glyphs) __asm__("FUN_001f7090");

void font_print_window(struct FontWindow *window, u64 color, u8 *text, s32 character_limit, s64 texture, struct Glyph *glyphs) {
    s16 line_starts[32];
    s16 line_ends[32];
    s16 line_colors[32];
    s32 wrap_width;
    s32 initial_wrap_width;
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
    s32 y;
    s32 line_length;
    s32 width;
    f32 draw_x;
    struct FontWindow *win;

    vu1_set_scissor(window->left, window->right - 1, window->top, window->bottom - 1);
    font_window_active = 1;
    if ((window->flags ^ 1) & 1) {
        wrap_width = window->right - window->x;
    } else {
        right_width = window->right - window->x;
        left_width = window->x - window->left;
        if (right_width < left_width) {
            left_width = right_width;
        }
        wrap_width = left_width * 2;
    }
    initial_wrap_width = wrap_width;
    current_color = 0;
    using_initial_width = 0;
    initial_line_count = 0;
    last_line_width = 0;
    balance_divisor = 3;
retry:
    line_count = 0;
    position = 0;
    while (position != character_limit && text[position] != 0) {
            next_line_count = line_count + 1;
            line_starts[line_count] = position;
            line_colors[line_count] = current_color;
            break_position = position;
            line_width = 0;
            if (wrap_width > 0) {
                do {
                    if (text[position] == ' ' || text[position] < 0x10) {
                        break_position = position;
                    }
                    if (font_color_codes_enabled[0] != 0 && (text[position] >= 8 && text[position] < 0x10)) {
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
            line_count = next_line_count;
            if (text[position] == 0) {
                /* Retail saves this width before testing whether to rebalance. */
                last_line_width = line_width;
                break;
            }
            position++;
    }
    if (!using_initial_width && initial_line_count == 0) {
        initial_line_count = line_count;
    }
    if (!using_initial_width && line_count >= 2) {
        if (initial_line_count < line_count) {
            wrap_width = initial_wrap_width;
            using_initial_width = 1;
            goto retry;
        }
        if (last_line_width < wrap_width / balance_divisor) {
            wrap_width -= 0x10;
            goto retry;
        }
    }

    win = window;
    line_length = win->line_height * line_count;
    win->text_width = 0;
    y = win->y;
    win->text_height = line_length;
    if (win->flags & 2) {
        y -= line_length >> 1;
    }
    for (line_index = 0; line_index < line_count; line_index++, y += win->line_height) {
        if (y + win->line_height < win->top) {
            continue;
        }
        if (win->bottom < y) {
            continue;
        }
        line_length = line_ends[line_index] - line_starts[line_index] + 1;
        width = measure_text_width(text + line_starts[line_index], line_length, glyphs);
        if (win->text_width < width) {
            win->text_width = width;
        }
        if (win->flags & 4) {
            continue;
        }
        font_palette_colors[0] = color;
        if (win->flags & 8) {
            if (win->flags & 1) {
                draw_x = (f32)(win->x - (width >> 1)) + (f32)win->offset_x * 0.0625f;
            } else {
                draw_x = (f32)win->x + (f32)win->offset_x * 0.0625f;
            }
            func_001F6638(font_palette_colors[line_colors[line_index]], text + line_starts[line_index], line_length, texture, glyphs,
                          draw_x, (f32)y + (f32)win->offset_y * 0.0625f, 1.0f);
        } else if (win->flags & 1) {
            font_print(win->x - (width >> 1), y, font_palette_colors[line_colors[line_index]], text + line_starts[line_index], line_length, texture, glyphs);
        } else {
            font_print(win->x, y, font_palette_colors[line_colors[line_index]], text + line_starts[line_index], line_length, texture, glyphs);
        }
    }
    font_window_active = 0;
    vu1_set_scissor(0, D_0013E500.width - 1, 0, D_0013E500.height - 1);
}

extern __typeof__(font_print_window) func_001F7090 __attribute__((alias("FUN_001f7090")));
#endif /* NON_MATCHING */
