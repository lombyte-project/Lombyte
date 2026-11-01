/* Ported from rac1-decomp (src/game/help.c, func_001FF1B0). */
#include "sda.h"
#include "rnc/ui/help.h"
#include "rnc/ui/text/font.h"

#include "rnc/ui/help/help_state.h"

#include "rnc/ui/help/message_entry.h"

extern HelpMessageEntry *help_msgs __asm__("D_0015F6A0") MACRO_ADDR;
extern int D_0015EE1D    MACRO_ADDR;
extern int D_0015EE1C    MACRO_ADDR;
#define D_0015EF1D_b (*(unsigned char *)&D_0015EE1D)
#define D_0015EF1C_b (*(unsigned char *)&D_0015EE1C)
extern void font_print_window_small(void *a, long b, void *c, int d) __asm__("func_001F75F0");
extern void draw_ui_frame(int, int, int, int, int) __asm__("func_001F5F18");
extern void
draw_textured_quad(int, int, int, int, int, int, int, int, long, long) __asm__("func_001F5450");
extern long get_effect_texture(int) __asm__("func_001F44B8");
/* Help_Draw: nothing unless a state is active, the help window is shown
   (help_state.started) and one of the fade flags is set. Then, by state, and only while
   D_0015EE1D is set: the growing frame (1), the prompt (2), the frame
   shrinking into the prompt icon with its fading quad (3), the full
   frame and the entry text, fading in (4) or out (6) through the text
   colour's alpha (4, 5, 6), or the closing frame (7). The fade flag is
   re-read in each case, as retail's per-case andi shows. */
void draw_help(void) __asm__("FUN_001fe980");

void draw_help(void) {
    enum {
        HELP_FRAME_OPENING = 1,
        HELP_PROMPT = 2,
        HELP_PROMPT_FRAME_TRANSITION = 3,
        HELP_TEXT_FADE_IN = 4,
        HELP_TEXT_STEADY = 5,
        HELP_TEXT_FADE_OUT = 6,
        HELP_FRAME_CLOSING = 7
    };
    enum {
        HELP_INACTIVE = 0
    };
    if (help_state.state == HELP_INACTIVE) {
        return;
    }
    if (help_state.started == 0) {
        return;
    }
    if (D_0015EF1D_b == 0 && D_0015EF1C_b == 0) {
        return;
    }
    switch (help_state.state) {
    case HELP_FRAME_OPENING:
        if (D_0015EF1D_b) {
            int frame_half_extent = help_state.timer * 4 + 8;

            help_state.frame_half_width = frame_half_extent;
            help_state.frame_half_height = frame_half_extent;
            draw_ui_frame(help_state.center_y - frame_half_extent,
                          help_state.center_y + frame_half_extent,
                          help_state.center_x - frame_half_extent,
                          help_state.center_x + frame_half_extent,
                          0x60);
        }
        break;
    case HELP_PROMPT:
        if (D_0015EF1D_b) {
            help_draw_prompt();
        }
        break;
    case HELP_PROMPT_FRAME_TRANSITION:
        if (D_0015EF1D_b) {
            int transition_half_width = (help_state.half_width - 0x20) * help_state.timer / 8 + 0x20;
            int transition_half_height = (help_state.half_height - 0x20) * help_state.timer / 8 + 0x20;
            int alpha;

            help_state.frame_half_width = transition_half_width;
            help_state.frame_half_height = transition_half_height;
            draw_ui_frame(help_state.center_y - transition_half_height,
                          help_state.center_y + transition_half_height,
                          help_state.center_x - transition_half_width,
                          help_state.center_x + transition_half_width,
                          0x60);
            alpha = (8 - help_state.timer) * 16;
            if (alpha < 0) {
                alpha = 0;
            }
            draw_textured_quad(help_state.center_x - 0x20,
                               help_state.center_y - 0x20,
                               0x40,
                               0x40,
                               0,
                               0,
                               0x40,
                               0x40,
                               (alpha << 24) | 0x808080,
                               get_effect_texture(4));
        }
        break;
    case HELP_TEXT_FADE_IN:
    case HELP_TEXT_STEADY:
    case HELP_TEXT_FADE_OUT:
        if (D_0015EF1D_b) {
            short win[12];
            int   col = 0x80FFA888;
            char *text;

            help_state.frame_half_width = help_state.half_width;
            help_state.frame_half_height = help_state.half_height;
            draw_ui_frame(help_state.center_y - help_state.half_height,
                          help_state.center_y + help_state.half_height,
                          help_state.center_x - help_state.half_width,
                          help_state.center_x + help_state.half_width,
                          0x60);
            if (help_state.state == HELP_TEXT_FADE_IN) {
                col = 0xFFA888 | (help_state.timer << 29);
            } else if (help_state.state == HELP_TEXT_FADE_OUT) {
                col = ((4 - help_state.timer) << 29) | 0xFFA888;
            }
            text = help_msgs[help_state.msg].text;
            font_set_window(
                (void *)win, 0xF0, 0x1E0, 0x2C, 0x1D4, 0x100, help_state.center_y, 0x10, 3);
            font_print_window_small((void *)win, col, text, -1);
        }
        break;
    case HELP_FRAME_CLOSING:
        if (D_0015EF1D_b) {
            int closing_half_width = help_state.frame_half_width -
                    (help_state.frame_half_width - 8) * help_state.timer / 8;
            int closing_half_height = help_state.frame_half_height -
                    (help_state.frame_half_height - 8) * help_state.timer / 8;

            draw_ui_frame(help_state.center_y - closing_half_height,
                          help_state.center_y + closing_half_height,
                          help_state.center_x - closing_half_width,
                          help_state.center_x + closing_half_width,
                          (8 - help_state.timer) * 12);
        }
        break;
    }
}

extern __typeof__(draw_help) func_001FE980 __attribute__((alias("FUN_001fe980")));
