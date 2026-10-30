/* Level 02 HUD counter: the update and draw callbacks of the counter skid_mcmarx queues (update_hud_counter_transition, draw_hud_counter). */
#include "types.h"
#include "asm.h"
#include "rnc/globals.h"
#include "rnc/ui/hud/get_icon_frame.h"
#include "rnc/ui/hud/sprites/draw_hud_sprite.h"
#include "rnc/ui/hud/sprites/draw_hud_sprite_flipped.h"
#include "rnc/overlay/shared/ui/hud/hud_frame.h"
#include "rnc/math/fast_math.h"

/* Ported from rac1-decomp (src/overlays/l02_aridia/hud_0023D600.c: func_L02_0023D600), where it is exact; names translated to the US level program. */

void update_hud_counter_transition(char *transition_state) __asm__("FUN_L02_0023ccc8");

void update_hud_counter_transition(char *transition_state) {
    unsigned char *transition_counters = (unsigned char *)(transition_state + 0x70);
    if (*(int *)(transition_state + 0x7C) >= scale_game_frames(5)) {
        if (transition_counters[0] < scale_game_frames(8)) {
            transition_counters[0] = transition_counters[0] + 1;
        } else if (transition_counters[1] < scale_game_frames(8)) {
            transition_counters[1] = transition_counters[1] + 1;
        }
    } else {
        if (transition_counters[1] != 0 || transition_counters[0] != 0) {
            *(int *)(transition_state + 0x6C) = 1;
        }
        if (transition_counters[1] != 0) {
            transition_counters[1] = transition_counters[1] - 1;
        } else if (transition_counters[0] != 0) {
            transition_counters[0] = transition_counters[0] - 1;
        }
    }
    *(int *)(transition_state + 0x74) = **(int **)(transition_state + 0xC);
}

#include "rnc/overlay/hud.h"

extern int  hud_screen_position(HudElem *, int *, int *) __asm__("FUN_L00_00235a70");
extern int  sprintf(char *str, const char *fmt, ...) __asm__("FUN_00116248");
extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void font_print_right(s32, s32, s32, char *, s32) __asm__("FUN_001f6940");
extern s32  frame_size[] __asm__("D_0013E504");
extern char hud_counter_decimal_format[] __asm__("D_L02_0015F850");

#include "rnc/overlay/l02/hud_counter_globals.h"
#include "rnc/math/asm_helpers.h"

extern s32 hud_counter_bar_growth_divisor_gp __asm__("D_L02_0015F91C") __attribute__((sda));
extern s32 hud_counter_text_shade_divisor_gp __asm__("D_L02_0015F920") __attribute__((sda));
s32        D_L02_0015F944 = 0x18;
s32        D_L02_0015F948 = 0x1e0;
s32        D_L02_0015F950 = 0xe08080;
s32        D_L02_0015F954 = 0x80e08080;
s32        D_L02_0015F958 = 0x30;
s32        D_L02_0015F95C = 0x46;

/* Draws the counter: a stretched bar behind the icon and the count, on whichever side of the screen it sits. */
s32 draw_hud_counter(HudElem *hud_element) __asm__("FUN_L02_0023cda8");

s32 draw_hud_counter(HudElem *hud_element) {
    char           text[16];
    int            x;
    int            y;
    unsigned char *fade = hud_element->cnt;
    int            left;
    f32            grow;
    f32            shade;
    int            alpha;
    int            width;
    int            frame;
    int            bright;
    int            dark;
    int            full_bar_width;
    int            px;
    int            bar;
    int            bottom;

    if (fade[0] == 0) {
        return hud_element->w;
    }
    x = hud_element->unk50;
    y = hud_element->unk54;
    hud_screen_position(hud_element, &x, &y);
    left = x < 0x101;
    if (left) {
        x = D_L02_0015F944;
    } else {
        x = D_L02_0015F948;
    }
    bottom = frame_size[0];
    y = pal_mode != 0 ? bottom - 0x2A : bottom - 0x32;
    grow = convert_integer_to_float(fade[0]) /
           convert_integer_to_float(hud_counter_bar_growth_divisor_gp);
    if (1.0f < grow) {
        grow = 1.0f;
    } else if (grow < 0.0f) {
        grow = 0.0f;
    }
    shade = convert_integer_to_float(fade[1]) /
            convert_integer_to_float(hud_counter_text_shade_divisor_gp);
    if (1.0f < shade) {
        shade = 1.0f;
    } else if (shade < 0.0f) {
        shade = 0.0f;
    }
    sprintf(text, hud_counter_decimal_format, hud_element->unk74);
    alpha = truncate_float_to_s32((f32)truncate_float_to_s32(grow * 128.0f) * 0.7f);
    if (hud_element->unk08 < 10) {
        full_bar_width = D_L02_0015F958;
    } else {
        full_bar_width = D_L02_0015F95C;
    }
    px = x;
    bar = px + 4;
    width = truncate_float_to_s32((f32)full_bar_width * grow);
    if (left) {
        frame = get_icon_frame(0x7580, 1);
        draw_hud_sprite(frame, px - 0x1C, y, 0x20, 0x20, alpha);
        frame = get_icon_frame(0x7580, 0);
        draw_hud_sprite(frame, bar, y, width, 0x20, alpha);
        frame = get_icon_frame(0x7580, 1);
        draw_hud_sprite_flipped(frame, bar + width, y, 0x20, 0x20, alpha);
    } else {
        frame = get_icon_frame(0x7580, 1);
        draw_hud_sprite_flipped(frame, bar, y, 0x20, 0x20, alpha);
        px = bar - width;
        frame = get_icon_frame(0x7580, 0);
        draw_hud_sprite(frame, px, y, width, 0x20, alpha);
        frame = get_icon_frame(0x7580, 1);
        draw_hud_sprite(frame, px - 0x20, y, 0x20, 0x20, alpha);
    }
    if (left) {
        hud_frame(hud_element, hud_element->icon, x, y, 0, 0x80);
    } else {
        hud_frame(hud_element, hud_element->icon, x - 0x16, y, 0, 0x80);
    }
    bright = fast_tween_color(D_L02_0015F950, D_L02_0015F954, shade);
    dark = fast_tween_color(0, 0x80000000, shade);
    if (left) {
        font_print_right(x + 0x43, y + 9, dark, text, -1);
        font_print_right(x + 0x42, y + 8, bright, text, -1);
    } else {
        font_print_right(x - 0x19, y + 9, dark, text, -1);
        font_print_right(x - 0x1A, y + 8, bright, text, -1);
    }
    return hud_element->w;
}
