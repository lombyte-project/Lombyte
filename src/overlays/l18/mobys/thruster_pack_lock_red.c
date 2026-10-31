/* Level 18 thruster_pack_lock_red and its wait requests. */
#include "types.h"
#include "rnc/overlay/level_camera.h"
#include "rnc/audio/voice_pool.h"
#include "rnc/hero/hero.h"
#include "rnc/globals.h"
#include "rnc/math_consts.h"
#include "asm.h"
#include "rnc/mobys/moby.h"
#include "rnc/overlay/moby_anim.h"
#include "rnc/overlay/entities.h"
#include "rnc/game/usage_stats.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/overlay/level_spline.h"

#include "sda.h"
#include "rnc/game/game_globals.h"
#include "rnc/math/random.h"
#include "rnc/ui/frames/draw_ui_frame.h"

extern int scale_game_frames(int);

extern f32 approach_value(f32 *, f32, f32) __asm__("FUN_00213ed8");
extern float
advance_accelerated_scalar(float *, float *, float, float, float, float) __asm__("FUN_00213f38");

extern float l18_thruster_pack_lock_red_scale_multiplier __asm__("D_L18_00161A10")
    __attribute__((sda));
extern float
    l18_thruster_pack_lock_red_grounded_trigger_wait_height_offset __asm__("D_L18_00161A14")
        __attribute__((sda));
extern float l18_thruster_pack_lock_red_approach_height_offset __asm__("D_L18_00161A18")
    __attribute__((sda));
extern unsigned int  D_L18_00162390 __asm__("D_L18_00162390") __attribute__((sda));
extern unsigned int  D_0015EEA4 __asm__("D_0015EEA4") __attribute__((sda));
extern unsigned char menu_item_enabled_115 __asm__("D_0013D407") NOT_SDA;
/* Help message records (same layout as the usage counters): count, best play time, level mask. */
extern struct UsageStat D_00141968_7b20[] __asm__("D_00141968");
void                    draw_pulsing_timer_hud(struct Moby *moby) __asm__("FUN_L18_002d8098");
extern void enqueue_callback_list_1_overlay(void (*)(void), void *) __asm__("FUN_L18_002020c8");
extern void release_voice_slot(s32) __asm__("FUN_0022d798");
extern int  help_request(int, int) __asm__("FUN_L00_00203908");

extern int allocate_voice_for_target_entry(int, int, int) __asm__("FUN_0022da68");

void thruster_pack_lock_red_update(struct Moby *moby) __asm__("FUN_L18_002d7b20");

/* Moby class 586 */
void thruster_pack_lock_red_update(struct Moby *moby) {
    enum {
        STATE_INITIALIZE = 0,
        STATE_APPROACH_CONFIGURED_HEIGHT = 1,
        STATE_WAIT_FOR_GROUNDED_HERO_TRIGGER = 2,
        STATE_POST_WAIT_APPROACH_CONFIGURED_HEIGHT = 3,
        STATE_WAIT_FOR_DEPRESSION_TRIGGER = 4,
        STATE_SETTLE_DEPRESSED_HEIGHT = 5
    };
    char *thruster_pack_lock_red_private_vars = (char *)moby->pvars;
    int   index;

    switch (moby->state) {
    case STATE_INITIALIZE:
        *(float *)(thruster_pack_lock_red_private_vars + 0x64) = moby->pos.z;
        *(unsigned short *)(thruster_pack_lock_red_private_vars + 0x3E) |= 8;
        if (*(int *)(thruster_pack_lock_red_private_vars + 0x60) != 0) {
            moby->state = 1;
            moby->scale = *(float *)((char *)moby->pclass + 0x24) *
                          l18_thruster_pack_lock_red_scale_multiplier;
            return;
        }
        moby->state = 4;
        return;
    case STATE_APPROACH_CONFIGURED_HEIGHT:
    case STATE_POST_WAIT_APPROACH_CONFIGURED_HEIGHT:
        advance_accelerated_scalar(&moby->pos.z,
                                   (float *)(thruster_pack_lock_red_private_vars + 0x68),
                                   *(float *)(thruster_pack_lock_red_private_vars + 0x64) +
                                       l18_thruster_pack_lock_red_approach_height_offset,
                                   frame_time_sq * 20.0f,
                                   frame_time_sq * 40.0f,
                                   frame_time * 20.0f);
        break;
    case STATE_WAIT_FOR_GROUNDED_HERO_TRIGGER:
        advance_accelerated_scalar(
            &moby->pos.z,
            (float *)(thruster_pack_lock_red_private_vars + 0x68),
            *(float *)(thruster_pack_lock_red_private_vars + 0x64) +
                l18_thruster_pack_lock_red_grounded_trigger_wait_height_offset,
            frame_time_sq * 5.0f,
            frame_time_sq * 10.0f,
            frame_time * 20.0f);
        if (hero.air_frames.s == 0 && hero.ground_moby == moby && hero.state.current == 0x22) {
            moby->state = 3;
            allocate_voice_for_target_entry(0, 0, (int)moby);
            index = *(int *)(thruster_pack_lock_red_private_vars + 0x70);
            if (index != -1) {
                unsigned char *voice_slot_record =
                    (unsigned char *)((char *)&voice_pool) + index * 0x70;
                if (*(struct Moby **)(voice_slot_record + 0x88) == moby &&
                    voice_slot_record[0x74] != 0)
                    release_voice_slot(index);
            }
            *(int *)(thruster_pack_lock_red_private_vars + 0x70) = -1;
        } else if (hero.state.current != 0x72) {
            enqueue_callback_list_1_overlay(draw_pulsing_timer_hud, moby);
            if (fast_dec_timer((int *)(thruster_pack_lock_red_private_vars + 0x6C)) != 0)
                D_L18_00162390 = 1;
        }
        break;
    case STATE_WAIT_FOR_DEPRESSION_TRIGGER:
        if (hero.air_frames.s == 0 && hero.ground_moby == moby && hero.state.current == 0x22) {
            moby->state = 5;
            moby->command = 1;
            moby->pos.z -= 1.5f;
            if (current_level_index == 0x12)
                menu_item_enabled_115 = 1;
        }
        if (vector_distance_xy(&moby->pos, &hero.motion.pos) < 2.5f) {
            struct UsageStat *help = D_00141968_7b20;
            int               one = 1;
            if ((help[0x2B02].level_mask & (one << current_level_index)) == 0) {
                if (help[0x71].count != 0) {
                    if (scale_game_frames(D_0015EEA4) - help[0x71].unk2 * 600 >
                            (int)((float)scale_game_frames(0x12) * 60.0f) ||
                        help[0x71].unk2 * 600 == 0) {
                        help_request(0x2B02, 0x71);
                    } else if (scale_game_frames(D_0015EEA4) / 600 > help[0x71].unk2) {
                        help[0x71].unk2 = scale_game_frames(D_0015EEA4) / 600;
                    }
                } else {
                    help[0x71].count++;
                    if (scale_game_frames(D_0015EEA4) / 600 > help[0x71].unk2) {
                        help[0x71].unk2 = scale_game_frames(D_0015EEA4) / 600;
                    }
                    help[0x71].level_mask =
                        help[0x71].level_mask | (one << current_level_index) | 0x80000000;
                }
            }
        }
        break;
    case STATE_SETTLE_DEPRESSED_HEIGHT:
        if (approach_value((float *)&moby->pos.z,
                           *(float *)(thruster_pack_lock_red_private_vars + 0x64) - 1.5f,
                           0.15f) != 0.0f)
            return;
        moby->state = 6;
        break;
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D9358), where it is exact; names translated to the US level program. */

extern int voice_is_playing(void *, int) __asm__("FUN_L00_0028d8c0");
s32        allocate_voice_for_group_entry(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_0022dba0");

void request_thruster_pack_lock_red_wait(struct Moby *moby, int value) __asm__("FUN_L18_002d7f68");

void request_thruster_pack_lock_red_wait(struct Moby *moby, int wait_duration_frames) {
    char *red_lock_vars = (char *)moby->pvars;
    if (moby->state != 2) {
        moby->state = 2;
        *(int *)(red_lock_vars + 0x6C) = wait_duration_frames;
    }
    if (!voice_is_playing(moby, *(int *)(red_lock_vars + 0x70))) {
        *(int *)(red_lock_vars + 0x70) = allocate_voice_for_group_entry(0x12, 0, moby);
    }
}
/* Ported from rac1-decomp (src/overlays/l18_veldin2/vendor_002A8400.c: func_L18_002D93C0), where it is exact; names translated to the US level program. */

void end_thruster_pack_lock_red_wait(struct Moby *moby) __asm__("FUN_L18_002d7fd0");

void end_thruster_pack_lock_red_wait(struct Moby *moby) {
    int *red_lock_private_vars = (int *)moby->pvars;
    if (moby->state == 2) {
        int voice_slot_index;
        moby->state = 3;
        voice_slot_index = red_lock_private_vars[0x70 / 4];
        if (voice_slot_index != -1) {
            unsigned char *voice_slot_entry =
                (unsigned char *)((char *)&voice_pool) + voice_slot_index * 0x70;
            if (*(unsigned char **)(voice_slot_entry + 0x88) == moby && voice_slot_entry[0x74]) {
                release_voice_slot(voice_slot_index);
            }
        }
        red_lock_private_vars[0x70 / 4] = -1;
    }
}

/* Checks the moby's phase: 1 (and rearms it) when done, -1 when waiting with no timer, else 0. */
int poll_moby_state_and_rearm(struct Moby *moby) __asm__("FUN_L18_002d8050");

int poll_moby_state_and_rearm(struct Moby *moby) {
    int *moby_private_vars = (int *)moby->pvars;
    if (moby->state == 3) {
        moby->state = 1;
        return 1;
    }
    if (moby->state == 2 && moby_private_vars[0x6C / 4] == 0) {
        return -1;
    }
    return 0;
}

int         timer_text_x __asm__("D_L18_00161A1C") = 260;
int         timer_text_y __asm__("D_L18_00161A20") = 80;
int         timer_color_dim __asm__("D_L18_00161A24") = 0x80c0c0c0;
int         timer_color_bright __asm__("D_L18_00161A28") = 0x804040ff;
int         timer_frame_top __asm__("D_L18_00161A2C") = -5;
int         timer_frame_bottom __asm__("D_L18_00161A30") = 20;
int         timer_frame_left __asm__("D_L18_00161A34") = -50;
int         timer_frame_right __asm__("D_L18_00161A38") = 52;
int         D_L18_00161A3C = 96;
extern int  fast_tween_color(int, int, float) __asm__("FUN_001fa6e0");
extern void font_print_center(int a, int b, int c, int d, int e) __asm__("FUN_001f6af0");
extern void font_print_center_large(int a, int b, int c, int d, int e) __asm__("FUN_001f6c20");

/* Draws the level timer as "H:MM:SS.t" text with a pulsing colour. */
void draw_pulsing_timer_hud(struct Moby *moby) __asm__("FUN_L18_002d8098");

void draw_pulsing_timer_hud(struct Moby *moby) {
    char  timer_text[8];
    char *timer_state = (char *)moby->pvars;
    float color_blend_factor;
    int   col;

    draw_ui_frame(timer_frame_top + timer_text_y,
                  timer_frame_bottom + timer_text_y,
                  timer_frame_left + timer_text_x,
                  timer_frame_right + timer_text_x,
                  D_L18_00161A3C);
    color_blend_factor =
        fast_sin(convert_integer_to_float(game_frame % 60) / 60.0f * 6.28318f) * 3.0f + 0.5f;
    if (color_blend_factor > 1.0f) {
        color_blend_factor = 1.0f;
    } else if (color_blend_factor < 0.0f) {
        color_blend_factor = 0.0f;
    }
    col = fast_tween_color(timer_color_dim, timer_color_bright, color_blend_factor);
    timer_text[0] = 0x30;
    timer_text[1] = *(int *)(timer_state + 0x6c) / scale_game_frames(0xE10) + 0x30;
    timer_text[2] = 0x3A;
    timer_text[3] =
        *(int *)(timer_state + 0x6c) % scale_game_frames(0xE10) / scale_game_frames(0x258) + 0x30;
    timer_text[4] =
        *(int *)(timer_state + 0x6c) % scale_game_frames(0x258) / scale_game_frames(0x3C) + 0x30;
    timer_text[5] = 0x3A;
    timer_text[6] =
        *(int *)(timer_state + 0x6c) % scale_game_frames(0x3C) / scale_game_frames(6) + 0x30;
    timer_text[7] = truncate_float_to_s32((float)random_integer_below(10)) + 0x30;
    font_print_center_large(timer_text_x, timer_text_y, col, (int)timer_text, 8);
}
