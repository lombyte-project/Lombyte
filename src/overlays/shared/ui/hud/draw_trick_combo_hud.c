/* HUD: the trick combo panel. */
#include "types.h"
#include "asm.h"

#include "sda.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/ui/hud/get_icon_frame.h"
#include "rnc/mobys/animation/set_animation_parameter.h"
#include "rnc/ui/help/get_help_message_text.h"
#include "rnc/ui/hud/sprites/draw_hud_sprite.h"
#include "rnc/overlay/shared/ui/hud/hud_frame.h"
#include "rnc/ui/frames/draw_framed_text.h"

/* Ported from rac1-decomp (src/overlays/shared/hud_00263490.c: func_L05_00264C88), where it is exact; names translated to the US level program. */

extern int D_L05_0015F97C;

#include "rnc/overlay/shared/ui/hud/draw_trick_combo_hud_data/hud_release_held_element.h"

extern short D_L05_0015F978 __attribute__((sda));
void         hud_release_held_element(void) __asm__("FUN_L05_00264298");

void hud_release_held_element(void) {
    if (animation_parameter_target_id != -1) {
        set_animation_parameter(animation_parameter_target_id, 0);
        animation_parameter_target_id = -1;
    }
    D_L05_0015F97C = 0;
    *(int *)&D_L05_0015F978 = *(int *)&D_L05_0015F978 + 1;
}

typedef struct {
    u8  pad[0x48];
    s16 unk48;
    s16 unk4A;
    u8  pad2[0x7C - 0x4C];
    s32 unk7C;
} HudElemX;

extern void initialize_numeric_hud_values_and_dimensions(void *) __asm__("FUN_L00_00235c80");
extern s32  D_L05_0015FAFC;

void initialize_trick_combo_hud(HudElemX *hud_element) __asm__("FUN_L05_00262ae8");

void initialize_trick_combo_hud(HudElemX *hud_element) {
    s32  i;
    s32 *reset_slot;
    hud_element->unk7C = scale_game_frames(0xB4) + 0x1E;
    hud_element->unk48 = 0;
    hud_element->unk4A = 0;
    i = 3;
    reset_slot = &D_L05_0015FAFC;
    for (; i >= 0; i--) {
        *reset_slot-- = -1;
    }
    initialize_numeric_hud_values_and_dimensions(hud_element);
}

#include "rnc/hero/hero.h"

typedef struct {
    u8   pad0[0xC];
    s32 *value;
    u8   pad1[0x70 - 0x10];
    u8   timer;
    u8   delay;
    u8   state;
    u8   pad2;
    s32  smoothed;
} HudCounter;

extern void               approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern s32 D_L05_0015FAD4 MACRO_ADDR;
extern s32 D_L05_0015FAD8 MACRO_ADDR;
extern s32                D_L05_0015FAE0[4] MACRO_ADDR;
extern s32 D_L05_0015FAF0 MACRO_ADDR;
extern s32 D_L05_0015FAF4 MACRO_ADDR;
extern s32 D_L05_0015FAF8 MACRO_ADDR;
extern s32                D_L05_0015FAFC_macro __asm__("D_L05_0015FAFC") MACRO_ADDR;
extern s32                D_L05_0015FB00;

#include "rnc/overlay/shared/ui/hud/draw_trick_combo_hud_globals.h"

extern u8 trick_combo_hud_delay_reload_gp __asm__("D_L05_0015F87C") __attribute__((sda));

void update_trick_combo_hud(HudCounter *element) __asm__("FUN_L05_00262b58");

void update_trick_combo_hud(HudCounter *element) {
    enum {
        HUD_COUNTER_WAITING_FOR_COMBO = 0,
        HUD_COUNTER_TRACKING_COMBO = 1,
        HUD_COUNTER_RESULT_COUNTDOWN = 2,
        HUD_COUNTER_REWARD_COUNTDOWN = 3
    };
    f32 smoothed;
    u8 *state_bytes;
    s32 combo;

    smoothed = (f32)element->smoothed;
    approach_value(&smoothed, (f32)*element->value, 6.0f);
    state_bytes = &element->timer;
    element->smoothed = truncate_float_to_s32(smoothed);
    switch (state_bytes[2]) {
    case HUD_COUNTER_WAITING_FOR_COMBO: {
        struct Hero *player = &hero;
        if (player->air_frames.s < 6) {
            return;
        }
        combo = player->unk70C;
        if (!((f32)combo >= 1.0f)) {
            return;
        }
        D_L05_0015FAD4 = combo;
        D_L05_0015FAD8 = 0;
        {
            int k;
            for (k = 3; k >= 0; k--) {
                D_L05_0015FAE0[k] = 0;
            }
        }
        state_bytes[2] = 1;
        state_bytes[1] = trick_combo_hud_delay_reload_gp;
        state_bytes[0] = 0;
        break;
    }
    case HUD_COUNTER_TRACKING_COMBO: {
        struct Hero *flag_source;
        struct Hero *player;
        if (element->timer < 10) {
            element->timer++;
        }
        if (state_bytes[1] != 0) {
            state_bytes[1]--;
        }
        flag_source = &hero;
        D_L05_0015FAD8 = flag_source->tricks_landed[4];
        {
            int k;
            for (k = 0; k < 4; k++) {
                D_L05_0015FAE0[k] = flag_source->tricks_landed[k];
            }
        }
        player = &hero;
        if (D_L05_0015FAD4 < player->unk70C) {
            D_L05_0015FAD4 = player->unk70C;
            state_bytes[1] = trick_combo_hud_delay_reload_gp;
        }
        if (player->grounded_frames == 0) {
            if (!(player->state.current >= 0x6D && player->state.current <= 0x6F)) {
                return;
            }
        }
        D_L05_0015FB00 = 0;
        player->unk70C = 0;
        player->tricks_landed[4] = 0;
        {
            int  k;
            s32 *reset_slot = &player->tricks_landed[3];
            for (k = 3; k >= 0; k--) {
                *reset_slot-- = 0;
            }
        }
        {
            int k;
            for (k = 2; k >= 0; k--) {
                hero.unk700[k] = 0;
            }
        }
        if (hero.state.current == 0x6B) {
            if (D_L05_0015FAD8 >= 3) {
                D_L05_0015FAFC_macro = D_L05_0015FAF8;
                D_L05_0015FAF8 = D_L05_0015FAF4;
                D_L05_0015FAF4 = D_L05_0015FAF0;
                D_L05_0015FAF0 = -1;
            }
            state_bytes[2] = 3;
            state_bytes[1] = trick_combo_hud_delay_reload_gp;
            state_bytes[0] = scale_game_frames(0x78);
        } else {
            state_bytes[2] = 2;
            state_bytes[1] = trick_combo_hud_delay_reload_gp;
            state_bytes[0] = scale_game_frames(0x3C);
        }
        break;
    }
    case HUD_COUNTER_RESULT_COUNTDOWN:
    case HUD_COUNTER_REWARD_COUNTDOWN:
        if (state_bytes[0] == 0) {
            D_L05_0015FB00 = 0;
            D_L05_0015FAD4 = 0;
            D_L05_0015FAD8 = 0;
            {
                int k;
                for (k = 3; k >= 0; k--) {
                    D_L05_0015FAE0[k] = 0;
                }
            }
            state_bytes[2] = 0;
            state_bytes[0] = 0;
        } else if (state_bytes[2] == 3 && state_bytes[0] == scale_game_frames(0x6E)) {
            s32          points = D_L05_0015FB00;
            struct Hero *player = &hero;
            player->race_score += points;
            player->boost_frames += points * 5;
            if ((s32)((f32)scale_game_frames(0x11) * 60.0f) < player->boost_frames) {
                player->boost_frames = (s32)((f32)scale_game_frames(0x11) * 60.0f);
            }
        }
        state_bytes[0]--;
        break;
    }
}

extern int  sprintf(char *, const char *, ...) __asm__("FUN_00116248");
extern s32  fast_tween_color(s32, s32, f32) __asm__("FUN_001fa6e0");
extern void font_print_center_large(s32, s32, u64, char *, s32) __asm__("FUN_001f6c20");
extern s32  hud_screen_position(char *, s32 *, s32 *) __asm__("FUN_L00_00235a70");

extern s32 D_L05_0015FAE4 MACRO_ADDR;
extern s32 D_L05_0015FAE8 MACRO_ADDR;
extern s32 D_L05_0015FAEC MACRO_ADDR;
extern s32                trick_combo_base_reward_points[15] __asm__("D_L05_0017E918");
extern s32                frame_number __asm__("D_L05_0015F5CC") MACRO_ADDR;
extern char               D_L05_0015F860[];
extern char               trick_combo_pose_count_format[] __asm__("D_L05_0015F8D0");
extern char               trick_combo_trick_name_format[] __asm__("D_L05_0015F8E0");
extern char               trick_combo_single_pose_flips_format[] __asm__("D_L05_0015F8E8");
extern char               trick_combo_two_pose_flips_format[] __asm__("D_L05_00216C50");

#include "rnc/overlay/shared/ui/hud/draw_trick_combo_hud_data/draw_trick_combo_hud_2.h"

extern s32 trick_combo_result_text_color_gp __asm__("D_L05_0015F8A0") __attribute__((sda));
extern s32 trick_combo_pulsing_label_x_gp __asm__("D_L05_0015F8A4") __attribute__((sda));
extern s32 trick_combo_pulsing_label_y_gp __asm__("D_L05_0015F8A8") __attribute__((sda));

#include "rnc/overlay/shared/ui/hud/draw_trick_combo_hud_data/draw_trick_combo_hud.h"

extern s32 trick_combo_rotation_text_center_x_gp __asm__("D_L05_0015F8B4") __attribute__((sda));
extern s32 trick_combo_rotation_text_y_gp __asm__("D_L05_0015F8B8") __attribute__((sda));
extern s32 trick_combo_pose_count_center_x_gp __asm__("D_L05_0015F8BC") __attribute__((sda));
extern s32 trick_combo_pose_count_y_gp __asm__("D_L05_0015F8C0") __attribute__((sda));
extern s32 trick_combo_reward_text_x_gp __asm__("D_L05_0015F8C4") __attribute__((sda));
extern s32 trick_combo_reward_label_y_gp __asm__("D_L05_0015F8C8") __attribute__((sda));

s32 draw_trick_combo_hud(char *element) __asm__("FUN_L05_00262f50");

s32 draw_trick_combo_hud(char *element) {
    u8          *state_bytes = (u8 *)(element + 0x70);
    char         text[0x80];
    s32          pose_ids[4];
    s32          x, y;
    f32          delay_left, fade, pulse, blend;
    struct Hero *player;
    s32 delay_total, trick, repeats, tier, trick_index, shadow_color, color, fill_icon, frame_icon,
        i, bar_width, blink_period, blink_color, warning_timer;

    fade = (f32)state_bytes[0] / 10.0f;
    if (fade > 1.0f) {
        fade = 1.0f;
    }
    player = &hero;
    if (player->state.control_mode != 0x16) {
        return *(s32 *)(element + 0x58);
    }
    if (state_bytes[2] != 0) {
        sprintf(text, D_L05_0015F860, D_L05_0015FAD4 * 0x168);
        delay_left = convert_integer_to_float(state_bytes[1]);
        delay_total = scale_game_frames(*(s32 *)&trick_combo_hud_delay_reload_gp);
        blend = delay_left / (f32)delay_total;
        if (state_bytes[2] == 1 || state_bytes[2] == 3) {
            color = fast_tween_color(trick_combo_text_color_at_zero_delay_gp,
                                     trick_combo_text_color_at_full_delay_gp,
                                     blend);
        } else {
            color = trick_combo_result_text_color_gp;
        }
        shadow_color = truncate_float_to_s32(fade * 128.0f) << 24;
        color += shadow_color;
        if (state_bytes[2] == 1 || state_bytes[2] == 2) {
            font_print_center_large(trick_combo_rotation_text_center_x_gp + 1,
                                    trick_combo_rotation_text_y_gp + 1,
                                    shadow_color,
                                    text,
                                    -1);
            font_print_center_large(trick_combo_rotation_text_center_x_gp,
                                    trick_combo_rotation_text_y_gp,
                                    color,
                                    text,
                                    -1);
            sprintf(text, trick_combo_pose_count_format, D_L05_0015FAD8);
            font_print_center_large(trick_combo_pose_count_center_x_gp + 1,
                                    trick_combo_pose_count_y_gp + 1,
                                    shadow_color,
                                    text,
                                    -1);
            font_print_center_large(
                trick_combo_pose_count_center_x_gp, trick_combo_pose_count_y_gp, color, text, -1);
        } else if (state_bytes[2] == 3) {
            trick = -1;
            if (D_L05_0015FAD8 == 3) {
                trick = 0;
                if (D_L05_0015FAEC != 0) {
                    trick = 2;
                    if (D_L05_0015FAE8 == 0) {
                        trick = 1;
                    } else if (D_L05_0015FAE4 != 0) {
                        trick = 3;
                    }
                }
            } else if (D_L05_0015FAD8 == 4) {
                trick = 4;
            }
            if (trick >= 0) {
                if (D_L05_0015FAD4 >= 4) {
                    tier = 2;
                } else {
                    tier = D_L05_0015FAD4 >= 2;
                }
                trick_index = trick + tier * 5;
                D_L05_0015FAF0 = trick_index;
                D_L05_0015FB00 = trick_combo_base_reward_points[trick_index];
                repeats = 1;
                for (i = 1; i < 4; i++) {
                    if ((&D_L05_0015FAF0)[i] == D_L05_0015FAF0) {
                        repeats++;
                    }
                }
                D_L05_0015FB00 = D_L05_0015FB00 / repeats;
                sprintf(text, D_L05_0015F860, D_L05_0015FB00);
                draw_framed_text(trick_combo_reward_text_x_gp,
                                 trick_combo_reward_label_y_gp - 0x18,
                                 color,
                                 text);
                sprintf(text,
                        trick_combo_trick_name_format,
                        get_help_message_text(trick_index + 0x5092));
                draw_framed_text(
                    trick_combo_reward_text_x_gp, trick_combo_reward_label_y_gp, color, text);
            } else if (D_L05_0015FAD8 > 0) {
                s32 count = 0;
                s32 pose;
                for (pose = 0; pose < 4 && count < D_L05_0015FAD8; pose++) {
                    if (D_L05_0015FAE0[pose] != 0) {
                        pose_ids[count] = pose;
                        count++;
                    }
                }
                if (D_L05_0015FAD8 == 1) {
                    D_L05_0015FB00 = D_L05_0015FAD4 * 10;
                    sprintf(text, D_L05_0015F860, D_L05_0015FB00);
                    draw_framed_text(trick_combo_reward_text_x_gp,
                                     trick_combo_reward_label_y_gp - 0x18,
                                     color,
                                     text);
                    sprintf(text,
                            trick_combo_single_pose_flips_format,
                            get_help_message_text(pose_ids[0] + 0x517E),
                            D_L05_0015FAD4);
                    draw_framed_text(
                        trick_combo_reward_text_x_gp, trick_combo_reward_label_y_gp, color, text);
                } else {
                    D_L05_0015FB00 = D_L05_0015FAD4 * 20;
                    sprintf(text, D_L05_0015F860, D_L05_0015FB00);
                    draw_framed_text(trick_combo_reward_text_x_gp,
                                     trick_combo_reward_label_y_gp - 0x18,
                                     color,
                                     text);
                    sprintf(text,
                            trick_combo_two_pose_flips_format,
                            get_help_message_text(pose_ids[0] + 0x517E),
                            get_help_message_text(pose_ids[1] + 0x517E),
                            D_L05_0015FAD4);
                    draw_framed_text(
                        trick_combo_reward_text_x_gp, trick_combo_reward_label_y_gp, color, text);
                }
            }
        }
    }

    x = *(s32 *)(element + 0x50);
    y = *(s32 *)(element + 0x54);
    *(s32 *)(element + 0x58) = 0x100;
    *(s32 *)(element + 0x5C) = 0x40;
    hud_screen_position(element, &x, &y);
    bar_width = (*(s32 *)(element + 0x74) * 0xDD) / *(s32 *)(element + 0x8) + 0x1B;
    fill_icon = get_icon_frame(0x7558, 1);
    draw_cropped_hud_sprite(fill_icon, x, y, bar_width, 0x40, 0x80);
    frame_icon = get_icon_frame(0x7558, 0);
    draw_hud_sprite(frame_icon, x, y, 0x100, 0x40, 0x80);
    player = &hero;
    warning_timer = player->off_course_frames;
    if (scale_game_frames(0x78) < warning_timer) {
        blink_period = scale_game_frames(0x3C);
        pulse = convert_integer_to_float(frame_number % blink_period) / (f32)blink_period;
        pulse = fast_sin(pulse * 6.283180236816406f - 3.141590118408203f);
        blink_color = fast_tween_color(trick_combo_pulsing_label_trough_color_gp,
                                       trick_combo_pulsing_label_peak_color_gp,
                                       pulse * 0.5f + 0.5f);
        sprintf(text, get_help_message_text(0x531A));
        draw_framed_text(trick_combo_pulsing_label_x_gp + 1,
                         trick_combo_pulsing_label_y_gp + 1,
                         0x80000000,
                         text);
        draw_framed_text(
            trick_combo_pulsing_label_x_gp, trick_combo_pulsing_label_y_gp, blink_color, text);
    }
    return *(s32 *)(element + 0x58);
}

s32 D_L16_0015FAFC = 0;
s32 D_L16_0015FAD4 = 0;
s32 D_L16_0015FAD8 = 0;
s32 D_L16_0015FAE0[4] = {0};

s32 D_L16_0015FAF0 = 0;
s32 D_L16_0015FAF4 = 0;
s32 D_L16_0015FAF8 = 0;
s32 D_L16_0015FB00 = 0;
u8  D_L16_0015F87C = 0x14;
s32 D_L16_0015FAE4 = 0;
s32 D_L16_0015FAE8 = 0;
s32 D_L16_0015FAEC = 0;

s32 D_L16_0017E918[15] = {0xc8,
                          0xd7,
                          0xe1,
                          0xeb,
                          0x12c,
                          0x145,
                          0x15e,
                          0x177,
                          0x190,
                          0x1f4,
                          0x20d,
                          0x226,
                          0x23f,
                          0x258,
                          0x28a};

s32 D_L16_0015F8A0 = 0xff;
s32 D_L16_0015F8A4 = 0x100;
s32 D_L16_0015F8A8 = 0xb4;
s32 D_L16_0015F8B4 = 0x15e;
s32 D_L16_0015F8B8 = 0x1b;
s32 D_L16_0015F8BC = 0x15e;
s32 D_L16_0015F8C0 = 0x2f;
s32 D_L16_0015F8C4 = 0xfa;
s32 D_L16_0015F8C8 = 0x7d;

s32 D_L05_0015FAD4 = 0;

s32 D_L05_0015FAD8 = 0;

s32 D_L05_0015FAE0[4] = {0};

s32 D_L05_0015FAF0 = 0;

s32 D_L05_0015FAF4 = 0;

s32 D_L05_0015FAF8 = 0;

s32 D_L05_0015FAFC_macro __asm__("D_L05_0015FAFC") = 0;

s32 D_L05_0015FB00 = 0;

s32 D_L05_0015FAE4 = 0;

s32 D_L05_0015FAE8 = 0;

s32 D_L05_0015FAEC = 0;

s32 trick_combo_base_reward_points[15] __asm__("D_L05_0017E918") = {0xc8,
                                                                    0xd7,
                                                                    0xe1,
                                                                    0xeb,
                                                                    0x12c,
                                                                    0x145,
                                                                    0x15e,
                                                                    0x177,
                                                                    0x190,
                                                                    0x1f4,
                                                                    0x20d,
                                                                    0x226,
                                                                    0x23f,
                                                                    0x258,
                                                                    0x28a};

s32 trick_combo_result_text_color_gp __asm__("D_L05_0015F8A0") = 0xff;
