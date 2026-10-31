/* Level 06 timer display: draws the level timer as "H:MM:SS.t" text with a pulsing colour (for countdown_sequence_update). */
#include "types.h"
#include "qcopy.h"
#include "rnc/mobys/moby.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/math/random.h"
#include "rnc/ui/frames/draw_ui_frame.h"

/* Ported from rac1-decomp (src/overlays/shared/vendor_002FF000.c: func_L06_00303630), where it is exact; names translated to the US level program. */

extern int   level_frame_count __asm__("D_L06_0015F5CC");
extern short timer_text_center_x_gp __asm__("D_L06_001620C8") __attribute__((sda));
extern short timer_text_y_gp __asm__("D_L06_001620CC") __attribute__((sda));
extern short timer_text_color_at_zero_blend_gp __asm__("D_L06_001620D0") __attribute__((sda));
extern short timer_text_color_at_unit_blend_gp __asm__("D_L06_001620D4") __attribute__((sda));
extern short timer_frame_x_addend_gp __asm__("D_L06_001620D8") __attribute__((sda));
extern short timer_frame_y_addend_gp __asm__("D_L06_001620DC") __attribute__((sda));
extern short timer_frame_left_x_addend_gp __asm__("D_L06_001620E0") __attribute__((sda));
extern short timer_frame_right_x_addend_gp __asm__("D_L06_001620E4") __attribute__((sda));
extern short timer_frame_fill_high_byte_gp __asm__("D_L06_001620E8") __attribute__((sda));
extern int   fast_tween_color_local(int, int, float) __asm__("FUN_001fa6e0");
extern void  font_print_center(int a, int b, int c, int d, int e) __asm__("FUN_001f6af0");
extern void  font_print_center_large(int a, int b, int c, int d, int e) __asm__("FUN_001f6c20");

#define W(x) (*(int *)&(x))

/* Draws the level timer as "H:MM:SS.t" text with a pulsing colour. */
void draw_pulsing_timer_display(char *moby) __asm__("FUN_L06_003021d8");

void draw_pulsing_timer_display(char *moby) {
    char  timer_text[8];
    char *timer_moby_vars = ((char *)MOBY(moby)->pvars);
    float color_blend_factor;
    int   col;

    draw_ui_frame(W(timer_frame_x_addend_gp) + W(timer_text_y_gp),
                  W(timer_frame_y_addend_gp) + W(timer_text_y_gp),
                  W(timer_frame_left_x_addend_gp) + W(timer_text_center_x_gp),
                  W(timer_frame_right_x_addend_gp) + W(timer_text_center_x_gp),
                  W(timer_frame_fill_high_byte_gp));
    color_blend_factor =
        fast_sin(convert_integer_to_float(level_frame_count % 60) / 60.0f * 6.28318f) * 3.0f + 0.5f;
    if (color_blend_factor > 1.0f) {
        color_blend_factor = 1.0f;
    } else if (color_blend_factor < 0.0f) {
        color_blend_factor = 0.0f;
    }
    col = fast_tween_color_local(W(timer_text_color_at_zero_blend_gp),
                                 W(timer_text_color_at_unit_blend_gp),
                                 color_blend_factor);
    timer_text[0] = 0x30;
    timer_text[1] = *(int *)(timer_moby_vars + 0xB4) / scale_game_frames(0xE10) + 0x30;
    timer_text[2] = 0x3A;
    timer_text[3] =
        *(int *)(timer_moby_vars + 0xB4) % scale_game_frames(0xE10) / scale_game_frames(0x258) +
        0x30;
    timer_text[4] =
        *(int *)(timer_moby_vars + 0xB4) % scale_game_frames(0x258) / scale_game_frames(0x3C) +
        0x30;
    timer_text[5] = 0x3A;
    timer_text[6] =
        *(int *)(timer_moby_vars + 0xB4) % scale_game_frames(0x3C) / scale_game_frames(6) + 0x30;
    timer_text[7] = truncate_float_to_s32((float)random_integer_below(10)) + 0x30;
    font_print_center_large(W(timer_text_center_x_gp), W(timer_text_y_gp), col, (int)timer_text, 8);
}

short timer_text_center_x_gp __asm__("D_L06_001620C8") = 0xfa;
short timer_text_y_gp __asm__("D_L06_001620CC") = 0x177;
short timer_text_color_at_zero_blend_gp __asm__("D_L06_001620D0") = -16192;
short timer_text_color_at_unit_blend_gp __asm__("D_L06_001620D4") = 0x40ff;
short timer_frame_x_addend_gp __asm__("D_L06_001620D8") = -5;
short timer_frame_y_addend_gp __asm__("D_L06_001620DC") = 0x14;
short timer_frame_left_x_addend_gp __asm__("D_L06_001620E0") = -50;
short timer_frame_right_x_addend_gp __asm__("D_L06_001620E4") = 0x34;
short timer_frame_fill_high_byte_gp __asm__("D_L06_001620E8") = 0x60;
