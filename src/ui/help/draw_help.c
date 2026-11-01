/* Ported from rac1-decomp (src/game/help.c, func_001FF1B0). */
#include "sda.h"

#include "rnc/ui/help/window_state.h"

extern char help_window_state[] __asm__("D_001996D0");

#include "rnc/ui/help/message_entry.h"

extern HelpMessageEntry *help_msgs __asm__("D_0015F6A0") MACRO_ADDR;
extern int D_0015EE1D    MACRO_ADDR;
extern int D_0015EE1C    MACRO_ADDR;
extern void              help_draw_prompt(void) __asm__("FUN_001fe898");
#define D_0015EF1D_b (*(unsigned char *)&D_0015EE1D)
#define D_0015EF1C_b (*(unsigned char *)&D_0015EE1C)
extern void
font_set_window(void *arg0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8);
extern void font_print_window_small(void *a, long b, void *c, int d) __asm__("func_001F75F0");
extern void draw_ui_frame(int, int, int, int, int) __asm__("func_001F5F18");
extern void
draw_textured_quad(int, int, int, int, int, int, int, int, long, long) __asm__("func_001F5450");
extern long get_effect_texture(int) __asm__("func_001F44B8");
/* Help_Draw: nothing unless a state is active, the help window is shown
   (+0x30) and one of the fade flags is set. Then, by state, and only while
   D_0015EE1D is set: the growing frame (1), the prompt (2), the frame
   shrinking into the prompt icon with its fading quad (3), the full
   frame and the entry text, fading in (4) or out (6) through the text
   colour's alpha (4, 5, 6), or the closing frame (7). The fade flag is
   re-read in each case, as retail's per-case andi shows. */
void draw_help(void) __asm__("FUN_001fe980");

void draw_help(void) {
    if (((HelpWindowState *)help_window_state)->state == HELP_WINDOW_IDLE) {
        return;
    }
    if (((HelpWindowState *)help_window_state)->input_delay_elapsed == 0) {
        return;
    }
    if (D_0015EF1D_b == 0 && D_0015EF1C_b == 0) {
        return;
    }
    switch (((HelpWindowState *)help_window_state)->state) {
    case HELP_WINDOW_OPENING:
        if (D_0015EF1D_b) {
            int frame_half_extent = ((HelpWindowState *)help_window_state)->state_timer * 4 + 8;

            ((HelpWindowState *)help_window_state)->frame_half_width = frame_half_extent;
            ((HelpWindowState *)help_window_state)->frame_half_height = frame_half_extent;
            draw_ui_frame(((HelpWindowState *)help_window_state)->center_y - frame_half_extent,
                          ((HelpWindowState *)help_window_state)->center_y + frame_half_extent,
                          ((HelpWindowState *)help_window_state)->center_x - frame_half_extent,
                          ((HelpWindowState *)help_window_state)->center_x + frame_half_extent,
                          0x60);
        }
        break;
    case HELP_WINDOW_PROMPT:
        if (D_0015EF1D_b) {
            help_draw_prompt();
        }
        break;
    case HELP_WINDOW_EXPANDING:
        if (D_0015EF1D_b) {
            int frame_half_width =
                (((HelpWindowState *)help_window_state)->target_half_width - 0x20) *
                    ((HelpWindowState *)help_window_state)->state_timer / 8 +
                0x20;
            int frame_half_height =
                (((HelpWindowState *)help_window_state)->target_half_height - 0x20) *
                    ((HelpWindowState *)help_window_state)->state_timer / 8 +
                0x20;
            int alpha;

            ((HelpWindowState *)help_window_state)->frame_half_width = frame_half_width;
            ((HelpWindowState *)help_window_state)->frame_half_height = frame_half_height;
            draw_ui_frame(((HelpWindowState *)help_window_state)->center_y - frame_half_height,
                          ((HelpWindowState *)help_window_state)->center_y + frame_half_height,
                          ((HelpWindowState *)help_window_state)->center_x - frame_half_width,
                          ((HelpWindowState *)help_window_state)->center_x + frame_half_width,
                          0x60);
            alpha = (8 - ((HelpWindowState *)help_window_state)->state_timer) * 16;
            if (alpha < 0) {
                alpha = 0;
            }
            draw_textured_quad(((HelpWindowState *)help_window_state)->center_x - 0x20,
                               ((HelpWindowState *)help_window_state)->center_y - 0x20,
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
    case HELP_WINDOW_TEXT_FADE_IN:
    case HELP_WINDOW_TEXT_VISIBLE:
    case HELP_WINDOW_TEXT_FADE_OUT:
        if (D_0015EF1D_b) {
            short text_window[12];
            int   text_color = 0x80FFA888;
            char *text;

            ((HelpWindowState *)help_window_state)->frame_half_width =
                ((HelpWindowState *)help_window_state)->target_half_width;
            ((HelpWindowState *)help_window_state)->frame_half_height =
                ((HelpWindowState *)help_window_state)->target_half_height;
            draw_ui_frame(((HelpWindowState *)help_window_state)->center_y -
                              ((HelpWindowState *)help_window_state)->target_half_height,
                          ((HelpWindowState *)help_window_state)->center_y +
                              ((HelpWindowState *)help_window_state)->target_half_height,
                          ((HelpWindowState *)help_window_state)->center_x -
                              ((HelpWindowState *)help_window_state)->target_half_width,
                          ((HelpWindowState *)help_window_state)->center_x +
                              ((HelpWindowState *)help_window_state)->target_half_width,
                          0x60);
            if (((HelpWindowState *)help_window_state)->state == HELP_WINDOW_TEXT_FADE_IN) {
                text_color = 0xFFA888 | (((HelpWindowState *)help_window_state)->state_timer << 29);
            } else if (((HelpWindowState *)help_window_state)->state == HELP_WINDOW_TEXT_FADE_OUT) {
                text_color =
                    ((4 - ((HelpWindowState *)help_window_state)->state_timer) << 29) | 0xFFA888;
            }
            text = help_msgs[((HelpWindowState *)help_window_state)->message_index].text;
            font_set_window((void *)text_window,
                            0xF0,
                            0x1E0,
                            0x2C,
                            0x1D4,
                            0x100,
                            ((HelpWindowState *)help_window_state)->center_y,
                            0x10,
                            3);
            font_print_window_small((void *)text_window, text_color, text, -1);
        }
        break;
    case HELP_WINDOW_CLOSING:
        if (D_0015EF1D_b) {
            int frame_half_width = ((HelpWindowState *)help_window_state)->frame_half_width -
                                   (((HelpWindowState *)help_window_state)->frame_half_width - 8) *
                                       ((HelpWindowState *)help_window_state)->state_timer / 8;
            int frame_half_height =
                ((HelpWindowState *)help_window_state)->frame_half_height -
                (((HelpWindowState *)help_window_state)->frame_half_height - 8) *
                    ((HelpWindowState *)help_window_state)->state_timer / 8;

            draw_ui_frame(((HelpWindowState *)help_window_state)->center_y - frame_half_height,
                          ((HelpWindowState *)help_window_state)->center_y + frame_half_height,
                          ((HelpWindowState *)help_window_state)->center_x - frame_half_width,
                          ((HelpWindowState *)help_window_state)->center_x + frame_half_width,
                          (8 - ((HelpWindowState *)help_window_state)->state_timer) * 12);
        }
        break;
    }
}

extern __typeof__(draw_help) func_001FE980 __attribute__((alias("FUN_001fe980")));
