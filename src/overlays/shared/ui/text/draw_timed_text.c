/* Text: draws timed text lines. */
#include "types.h"
#include "asm.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/ui/help/get_help_message_text.h"
#include "rnc/ui/text/font/disable_font_color_codes.h"
#include "rnc/ui/text/font/enable_font_color_codes.h"
#include "rnc/ui/text/font/measure_text_width_regular.h"
#include "rnc/ui/frames/draw_stretchable_ui_frame.h"

extern void font_print_large(int, int, u64, char *, int) __asm__("FUN_001f6530");

void draw_timed_text_line(int x, int y, int mode, float scale, float alpha, char *text) __asm__("FUN_L00_001fcb28");

void draw_timed_text_line(int x, int y, int mode, float scale, float alpha, char *text) {
    enum {
        TEXT_ALIGN_CENTER = 0,
        TEXT_ALIGN_RIGHT = 1
    };
    int width, right, left;
    u32 shadow;
    s32 color;
    width = measure_text_width_regular(text, -1) * scale;
    left = x;
    right = left;
    switch (mode) {
    case TEXT_ALIGN_CENTER:
        left -= width >> 1;
        right = x + (width >> 1);
        break;
    case TEXT_ALIGN_RIGHT:
        left -= width;
        break;
    case -1:
        right = left + width;
        break;
    }
    draw_stretchable_ui_frame(
        left - 0x20, y - 0xE, right - left + 0x40, 0x1C, truncate_float_to_s32(scale * 112.0f));
    disable_font_color_codes();
    shadow = (int)(alpha * 128.0f) << 24;
    color = shadow + 0xE0E0E0;
    font_print_large(left + 1, y - 7, shadow, text, -1);
    font_print_large(left, y - 8, color, text, -1);
    enable_font_color_codes();
}
/* Ported from rac1-decomp (src/overlays/shared/draw_001F3A78.c: func_L00_001FCC50), where it is exact; names translated to the US level program. */

typedef struct {
    s32 text_id;
    s16 x;
    s16 y;
    s16 start;
    s16 duration;
    s16 mode;
    s16 separator_mode;
} LevelTextEntry;

void draw_timed_text(LevelTextEntry *entry, s32 frame) __asm__("FUN_L00_001fcca0");

void draw_timed_text(LevelTextEntry *timed_text_entry, s32 frame) {
    int            start = timed_text_entry->start;
    int            end = start + timed_text_entry->duration;
    float          scale, alpha;
    unsigned char *source;
    char           formatted_text[256];
    char          *text_write_cursor;
    int            y;
    int            k;

    if (frame < start)
        return;
    if (end < frame)
        return;
    scale = 1.0f;
    if (frame < start + 16)
        scale = (float)(frame - start) * 0.0625f;
    if (end - 8 < frame)
        scale = (float)(end - frame) * 0.125f;
    alpha = 1.0f;
    if (frame < start + 32) {
        k = frame - 16;
        alpha = (float)(k - start) * 0.0625f;
        if (alpha < 0.0f)
            alpha = 0.0f;
    }
    if (end - 16 < frame) {
        k = frame + 8;
        alpha = (float)(end - k) * 0.125f;
        if (alpha < 0.0f)
            alpha = 0.0f;
    }
    source = (unsigned char *)get_help_message_text(timed_text_entry->text_id);
    if (timed_text_entry->separator_mode == 1) {
        text_write_cursor = formatted_text;
    joined_text:
        while (*source != 0) {
            if (*source == 1) {
                *text_write_cursor++ = ' ';
                *text_write_cursor++ = '-';
                *text_write_cursor++ = ' ';
                while (*source == 1)
                    source++;
                if (*source != 0)
                    goto joined_text;
                text_write_cursor -= 3;
            } else {
                *text_write_cursor++ = *source++;
            }
        }
        {
            int x = timed_text_entry->x;
            int line_y = timed_text_entry->y;
            int mode = timed_text_entry->mode;
            *text_write_cursor = 0;
            draw_timed_text_line(x, line_y, mode, scale, alpha, formatted_text);
        }
    } else {
        text_write_cursor = formatted_text;
        y = timed_text_entry->y;
        while (*source != 0) {
            if (*source == 1) {
                *text_write_cursor = 0;
                draw_timed_text_line(
                    timed_text_entry->x, y, timed_text_entry->mode, scale, alpha, formatted_text);
                y += 26;
                text_write_cursor = formatted_text;
                while (*source == 1)
                    source++;
                if (*source == 0)
                    break;
            } else {
                *text_write_cursor++ = *source++;
            }
        }
        if (text_write_cursor > formatted_text) {
            *text_write_cursor = 0;
            draw_timed_text_line(
                timed_text_entry->x, y, timed_text_entry->mode, scale, alpha, formatted_text);
        }
    }
}
