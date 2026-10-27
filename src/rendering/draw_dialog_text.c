#include "types.h"
#include "sda.h"
#include "rnc/globals.h"
#include "rnc/rendering/screen.h"
#include "rnc/ui/text/text_region.h"
#include "rnc/gameplay/hero.h"
#include "rnc/ui/text/font_metrics.h"
#include "rnc/data_labels.h"
#include "rnc/gameplay/state/options.h"
#include "rnc/gameplay/state/game_globals.h"

/* State of the level's message box (D_00193300). */
typedef struct {
    s32 state;    /* 0x00 */
    s32 timer;    /* 0x04: fade-in frames */
    char *title;  /* 0x08 */
    char *line1;  /* 0x0C */
    char *line2;  /* 0x10 */
    u8 pad14[8];
    s32 choice;   /* 0x1C */
    s32 unk20;    /* 0x20 */
    s32 unk24;    /* 0x24 */
} MessageBox;

extern MessageBox freeze __asm__("D_00193300");
extern s32 freeze_wave_period __asm__("D_0015F520") __attribute__((sda));
extern char freeze_default_text[] __asm__("D_0015F548");
extern char freeze_fmt_scsc[] __asm__("D_0015F550");
extern char freeze_fmt_indent[] __asm__("D_0015F560");
extern char freeze_fmt_time[] __asm__("D_0015F590");
extern char freeze_fmt_score[] __asm__("D_0015F5A0");

extern void memset(void *, s32, s32);
extern s32 sprintf(char *, const char *, ...);
extern void strncpy(void *, char *, s32);
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern s32 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void draw_ui_frame(s32, s32, s32, s32, s32) __asm__("func_001F5F18");
extern void draw_outlined_rect(s32, s32, s32, s32, s32) __asm__("func_001F6060");
extern s32 font_print_center(s32, s32, s64, char *, s32) __asm__("func_001F6AF0");
extern void font_print_window(struct TextRegion *, s64, void *, s32, s32, u8 *) __asm__("FUN_001f7090");
extern void font_print_window_regular(struct TextRegion *, s32, char *, s32) __asm__("func_001F7580");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 convert_integer_to_float(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 fast_tween_color(s32, s32, f32) __asm__("func_001FA6E0");
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern s32 find_valid_animation_frame_index_alt(s32, s32) __asm__("func_001FF960");
extern s64 get_frame_texture(s32) __asm__("func_001FFA10");
extern void draw_hud_sprite(s32, s32, s32, s32, s32, s32) __asm__("func_001FFC30");
/* Same registers as (w, h, tex, x, y, cx, cy, angle); floats are listed first so they are evaluated first. */
extern void draw_rotated_sprite(f32 x, f32 y, f32 cx, f32 cy, f32 angle, s32 w, s32 h, s64 tex) __asm__("FUN_00200600");
extern void vu1_add_g_sregister(s32, s64) __asm__("FUN_00233980");

#define SCRATCHPAD ((u8 *)0x70000000)

void draw_dialog_text(void) __asm__("FUN_001fbc50");

extern s32 freeze_ans2 __asm__("D_0015F4E8") __attribute__((sda));
extern s32 freeze_ans1 __asm__("D_0015F4EC") __attribute__((sda));
#include "rnc/rendering/draw_dialog_text_globals.h"
extern s32 freeze_bevel_a_text __asm__("D_0015F508") __attribute__((sda));
extern s32 freeze_stats_text __asm__("D_0015F51C") __attribute__((sda));
extern char D_0015F568[];
extern char D_0015F570[];
extern char D_0015F578[];
extern char D_0015F580[];
extern char D_0015F588[];
struct TextRegion D_001E78F0 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};
struct TextRegion D_001E7908 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};
void draw_dialog_text(void) {
    enum {
        FREEZE_SCALED_OUTLINE_DIALOG = 0,
        FREEZE_TITLE_DIALOG = 2,
        FREEZE_MODE_MESSAGE_DIALOG = 3,
        FREEZE_SPLIT_TEXT_ICON_DIALOG = 5,
        FREEZE_CHOICE_MESSAGE_DIALOG = 6
    };
    enum {
        FREEZE_CHOICE_PAIRED_FOOTER_MESSAGE = 0,
        FREEZE_CHOICE_SHARED_PAIRED_FOOTER_MESSAGE = 1,
        FREEZE_CHOICE_ALTERNATE_PAIRED_FOOTER_MESSAGE = 2,
        FREEZE_CHOICE_CENTERED_FOOTER_MESSAGE = 3
    };
    enum {
        MODE_FREEZE_TIMER_GATED_CENTERED_FOOTER_MESSAGE = 2,
        MODE_FREEZE_BOOT_FLAG_GATED_PAIRED_OR_CENTERED_FOOTER_MESSAGE = 3,
        MODE_FREEZE_SHARED_BOOT_FLAG_GATED_PAIRED_OR_CENTERED_FOOTER_MESSAGE = 4,
        MODE_FREEZE_FORMATTED_CENTERED_FOOTER_MESSAGE = 5,
        MODE_FREEZE_ALTERNATE_LEFT_PAIRED_FOOTER_MESSAGE = 6,
        MODE_FREEZE_BODY_ONLY_MESSAGE = 7,
        MODE_FREEZE_SHARED_BODY_ONLY_MESSAGE = 8,
        MODE_FREEZE_FLAG_GATED_PAIRED_OR_CENTERED_FOOTER_MESSAGE = 12,
        MODE_FREEZE_SHARED_ALTERNATE_LEFT_PAIRED_FOOTER_MESSAGE = 13,
        MODE_FREEZE_CENTERED_FOOTER_MESSAGE = 17,
        MODE_FREEZE_ALTERNATE_CENTERED_FOOTER_MESSAGE = 18,
        MODE_FREEZE_CONDITIONAL_FORMATTED_PAIRED_OR_CENTERED_FOOTER_MESSAGE = 19,
        MODE_FREEZE_PAIRED_FOOTER_MESSAGE = 23,
        MODE_FREEZE_FORMATTED_PAIRED_FOOTER_MESSAGE = 24
    };
    char formatted_dialog_text[0x200];
    struct TextRegion box;

    setup_gif_paging(0);
    switch (freeze.state) {
    case FREEZE_SPLIT_TEXT_ICON_DIALOG: {
        u8 *text_cursor;
        s32 color;
        s32 tint;
        s32 y;
        s32 top;
        s32 mid;
        f32 alpha;
        f32 icon_y_subpixels;
        f32 angle;
        f32 pivot;

        alpha = 1.0f - (f32)freeze.timer / (f32)scale_game_frames(30);
        draw_ui_frame(0x50, 0x154, 0x60, 0x1A0, (s32)(alpha * 80.0f));
        box = D_001E78F0;
        color = fast_tween_color(freeze_fading_dialog_text_base_color, freeze_fading_dialog_text_blend_target_color, alpha);
        strncpy(SCRATCHPAD, get_help_message_text(0x4E2B), 0x400);
        text_cursor = SCRATCHPAD;
        while (*text_cursor >= 2) {
            text_cursor++;
        }
        if (*text_cursor == 1) {
            do {
                *text_cursor++ = 0;
            } while (*text_cursor == 1);
        }
        font_print_window(&box, color, SCRATCHPAD, -1, get_effect_texture(1), normal_font_metrics);
        pivot = 272.0f;
        box.flags |= 4;
        y = box.anchor_y;
        y += box.rendered_height;
        font_print_window(&box, color, text_cursor, -1, get_effect_texture(1), normal_font_metrics);
        box.flags ^= 4;
        box.anchor_y = 0x136 - box.rendered_height;
        font_print_window(&box, color, text_cursor, -1, get_effect_texture(1), normal_font_metrics);
        tint = fast_tween_color(0x20FFFF, 0x8020FFFF,
                            1.0f - (f32)freeze.unk24 / (f32)scale_game_frames(30));
        font_print_center(0x100, 0x140, tint, get_help_message_text(0x524A), -1);
        mid = (y + box.anchor_y) >> 1;
        vu1_add_g_sregister(0x47, 0x3004B);
        top = mid - 0x20;
        draw_hud_sprite(find_valid_animation_frame_index_alt(0x755D, 0), 0xE0, top, 0x40, 0x40, 0x80);
        icon_y_subpixels = (f32)(mid << 4);
        angle = (f32)(game_frame_counter % 55) * -6.2831855f / 55.0f;
        draw_rotated_sprite(4096.0f, icon_y_subpixels, pivot, pivot, angle, 0x40, 0x40,
                            get_frame_texture(find_valid_animation_frame_index_alt(0x755D, 1)));
        break;
    }
    case FREEZE_MODE_MESSAGE_DIALOG: {
        char *text;
        s32 left;
        s32 right;
        s32 mid;
        s32 tint;
        s32 y;
        s32 dialog_height;
        f32 alpha;

        text = freeze_default_text;
        left = 0;
        mid = 0;
        right = 0;
        switch (mode_freeze_state) {
        case MODE_FREEZE_ALTERNATE_LEFT_PAIRED_FOOTER_MESSAGE:
            left = 0x524F;
            right = 0x524B;
            text = get_help_message_text(0x4FAF);
            break;
        case MODE_FREEZE_FLAG_GATED_PAIRED_OR_CENTERED_FOOTER_MESSAGE:
            if (!(mode_freeze_flags & 2)) {
                mid = 0x4FA8;
                text = get_help_message_text(0x4FA7);
                break;
            }
        case MODE_FREEZE_SHARED_ALTERNATE_LEFT_PAIRED_FOOTER_MESSAGE:
            left = 0x524F;
            right = 0x524B;
            text = get_help_message_text(0x4FAD);
            break;
        case 10:
        case 11:
            text = get_help_message_text(0x4FC1);
            break;
        case MODE_FREEZE_BODY_ONLY_MESSAGE:
        case MODE_FREEZE_SHARED_BODY_ONLY_MESSAGE:
            text = get_help_message_text(0x4FB7);
            break;
        case 14:
        case 15:
            text = get_help_message_text(0x4FB8);
            break;
        case MODE_FREEZE_CENTERED_FOOTER_MESSAGE:
            mid = 0x524A;
            text = get_help_message_text(0x4FBA);
            break;
        case MODE_FREEZE_ALTERNATE_CENTERED_FOOTER_MESSAGE:
            mid = 0x524A;
            text = get_help_message_text(0x4FBC);
            break;
        case MODE_FREEZE_TIMER_GATED_CENTERED_FOOTER_MESSAGE:
            mid = 0x524A;
            text = get_help_message_text(0x4FA6);
            if (freeze.timer != 0) {
                mid = 0;
            }
            break;
        case MODE_FREEZE_CONDITIONAL_FORMATTED_PAIRED_OR_CENTERED_FOOTER_MESSAGE:
            if (mode_freeze_flags & 4) {
                mid = 0x4FA8;
                text = get_help_message_text(0x4FA7);
            } else if (boot_level_active != 0) {
                left = 0x524E;
                right = 0x524B;
                sprintf(formatted_dialog_text, freeze_fmt_scsc, get_help_message_text(0x4FA9), 1, 1, get_help_message_text(0x4FAA));
                text = formatted_dialog_text;
            } else {
                mid = 0x4FA8;
                text = get_help_message_text(0x4FA9);
            }
            break;
        case 21:
            mid = 0x524A;
            text = get_help_message_text(0x4FBD);
            break;
        case 20:
            mid = 0x524A;
            text = get_help_message_text(0x4FBB);
            break;
        case MODE_FREEZE_FORMATTED_PAIRED_FOOTER_MESSAGE:
            left = 0x524E;
            right = 0x524B;
            sprintf(formatted_dialog_text, freeze_fmt_scsc, get_help_message_text(0x4FAE), 1, 1, get_help_message_text(0x4FAA));
            text = formatted_dialog_text;
            break;
        case MODE_FREEZE_PAIRED_FOOTER_MESSAGE:
            left = 0x524E;
            right = 0x524B;
            text = get_help_message_text(0x4FB1);
            break;
        case MODE_FREEZE_BOOT_FLAG_GATED_PAIRED_OR_CENTERED_FOOTER_MESSAGE:
        case MODE_FREEZE_SHARED_BOOT_FLAG_GATED_PAIRED_OR_CENTERED_FOOTER_MESSAGE:
            if (boot_level_active != 0 && (mode_freeze_flags & 2)) {
                left = 0x524E;
                right = 0x524B;
                text = get_help_message_text(0x4FAB);
            } else {
                mid = 0x4FA8;
                text = get_help_message_text(0x4FAC);
            }
            break;
        case MODE_FREEZE_FORMATTED_CENTERED_FOOTER_MESSAGE:
            mid = 0x4FA8;
            sprintf(formatted_dialog_text, freeze_fmt_scsc, get_help_message_text(0x4FB0), 1, 1, get_help_message_text(0x4FA7));
            text = formatted_dialog_text;
            break;
        }
        box = (struct TextRegion){0, screen_extent.height, 0x60, 0x1A0, 0x100, 0x68, 0, 0, 0x10, 5};
        font_print_window_regular(&box, 0, text, -1);
        dialog_height = box.rendered_height + 0x28;
        y = (screen_extent.height - dialog_height) >> 1;
        box.bottom = y + dialog_height;
        box.anchor_y = y + 4;
        box.top = y;
        alpha = 1.0f - (f32)freeze.timer / (f32)scale_game_frames(30);
        draw_ui_frame(box.top, box.bottom, 0x60, 0x1A0, (s32)(alpha * 80.0f));
        box.flags ^= 4;
        font_print_window_regular(&box, fast_tween_color(freeze_fading_dialog_text_base_color, freeze_fading_dialog_text_blend_target_color, alpha), text, -1);
        tint = fast_tween_color(0x20FFFF, 0x8020FFFF,
                            1.0f - (f32)freeze.unk24 / (f32)scale_game_frames(30));
        if (left != 0) {
            font_print_center(0xCA, box.bottom - 0x14, tint, get_help_message_text(left), -1);
        }
        if (right != 0) {
            font_print_center(0x135, box.bottom - 0x14, tint, get_help_message_text(right), -1);
        }
        if (mid != 0) {
            font_print_center(0x100, box.bottom - 0x14, tint, get_help_message_text(mid), -1);
        }
        break;
    }
    case FREEZE_CHOICE_MESSAGE_DIALOG: {
        s32 text_color;
        s32 button_color;
        s32 id;
        f32 alpha;

        box = D_001E7908;
        alpha = 1.0f - (f32)freeze.timer / (f32)scale_game_frames(30);
        draw_ui_frame(0x64, 0x12C, 0x60, 0x1A0, (s32)(alpha * 80.0f));
        text_color = fast_tween_color(freeze_fading_dialog_text_base_color, freeze_fading_dialog_text_blend_target_color, alpha);
        button_color = fast_tween_color(0x20FFFF, 0x8020FFFF, alpha);
        switch (freeze.choice) {
        case FREEZE_CHOICE_PAIRED_FOOTER_MESSAGE:
        case FREEZE_CHOICE_SHARED_PAIRED_FOOTER_MESSAGE:
            font_print_center(0xCA, 0x118, button_color, get_help_message_text(0x524E), -1);
            font_print_center(0x135, 0x118, button_color, get_help_message_text(0x524B), -1);
            id = 0x522A;
            break;
        case FREEZE_CHOICE_ALTERNATE_PAIRED_FOOTER_MESSAGE:
            font_print_center(0xCA, 0x118, button_color, get_help_message_text(0x524E), -1);
            font_print_center(0x135, 0x118, button_color, get_help_message_text(0x524B), -1);
            id = 0x522B;
            break;
        case FREEZE_CHOICE_CENTERED_FOOTER_MESSAGE:
            font_print_center(0x100, 0x118, button_color, get_help_message_text(0x524A), -1);
            id = 0x522C;
            break;
        default:
            id = 0;
            break;
        }
        if (id != 0) {
            font_print_window(&box, text_color, get_help_message_text(id), -1, get_effect_texture(1), normal_font_metrics);
        }
        break;
    }
    case FREEZE_SCALED_OUTLINE_DIALOG: {
        char line[0x40];
        s32 pulse;
        s32 shared_dialog_text_color;
        s32 matched_record_text_color;
        s32 unmatched_record_text_color;
        s32 best;
        s32 record;
        s32 unit;
        s32 rem;
        s32 per;
        s32 min;
        s32 sec;
        s32 hund;
        f32 scale;
        f32 color_blend_fraction;

        pulse = fast_tween_color(freeze_outline_pulse_base_color, freeze_outline_pulse_blend_target_color,
                             fast_sin((f32)(game_frame_counter % freeze_wave_period) / convert_integer_to_float(freeze_wave_period) * 6.28318f - 3.14159f) *
                                     0.5f +
                                 0.5f);
        scale = (f32)freeze.unk20 * 0.125f;
        if (scale > 1.0f) {
            scale = 1.0f;
        } else if (scale < 0.1f) {
            scale = 0.1f;
        }
        shared_dialog_text_color = freeze_shared_dialog_text_base_color;
        matched_record_text_color = freeze_matched_record_text_base_color;
        unmatched_record_text_color = freeze_unmatched_record_text_base_color;
        if (freeze.unk24 != 0) {
            color_blend_fraction = (f32)freeze.unk24 * 0.125f;
            if (color_blend_fraction > 1.0f) {
                color_blend_fraction = 1.0f;
            } else if (color_blend_fraction < 0.0f) {
                color_blend_fraction = 0.0f;
            }
            shared_dialog_text_color = fast_tween_color(freeze_shared_dialog_text_base_color, freeze_shared_dialog_text_blend_target_color, color_blend_fraction);
            matched_record_text_color = fast_tween_color(freeze_matched_record_text_base_color, freeze_matched_record_text_blend_target_color, color_blend_fraction);
            unmatched_record_text_color = fast_tween_color(freeze_unmatched_record_text_base_color, freeze_unmatched_record_text_blend_target_color, color_blend_fraction);
        }
        if (hero.race_lap < 3) {
            draw_outlined_rect(freeze_three_line_outline_center_y - truncate_float_to_s32((f32)freeze_three_line_outline_half_height * scale),
                         freeze_three_line_outline_center_y + truncate_float_to_s32((f32)freeze_three_line_outline_half_height * scale),
                         freeze_three_line_outline_center_x - truncate_float_to_s32((f32)freeze_three_line_outline_half_width * scale),
                         freeze_three_line_outline_center_x + truncate_float_to_s32((f32)freeze_three_line_outline_half_width * scale), pulse);
            font_print_center(0x100, freeze_bevel_a_text, shared_dialog_text_color, get_help_message_text(0x4F6E), -1);
            font_print_center(0x100, freeze_bevel_a_text + 0x18, shared_dialog_text_color, get_help_message_text(0x5249), -1);
            font_print_center(0x100, freeze_bevel_a_text + 0x30, shared_dialog_text_color, get_help_message_text(0x5248), -1);
        } else {
            draw_outlined_rect(freeze_stats_outline_center_y - truncate_float_to_s32((f32)freeze_stats_outline_half_height * scale),
                         freeze_stats_outline_center_y + truncate_float_to_s32((f32)freeze_stats_outline_half_height * scale),
                         freeze_stats_outline_center_x - truncate_float_to_s32((f32)freeze_stats_outline_half_width * scale),
                         freeze_stats_outline_center_x + truncate_float_to_s32((f32)freeze_stats_outline_half_width * scale), pulse);
            sprintf(line, freeze_fmt_indent, get_help_message_text(0x5240));
            if (hero.race_position == 1) {
                sprintf(line, D_0015F568, 1);
            } else if (hero.race_position == 2) {
                sprintf(line, D_0015F570, 2);
            } else if (hero.race_position == 3) {
                sprintf(line, D_0015F578, 3);
            } else {
                sprintf(line, D_0015F580, hero.race_position);
            }
            line[3] = ' ';
            if (hero.race_position == 1) {
                font_print_center(0x100, freeze_stats_text, matched_record_text_color, line, -1);
            } else {
                font_print_center(0x100, freeze_stats_text, unmatched_record_text_color, line, -1);
            }
            best = freeze_stats_time_ref[(current_level_index ^ 0x10) ? 0 : 1];
            if (best == hero.race_time_frames) {
                unit = pal_mode ? 3000 : 3600;
                per = unit / 60;
                min = best / unit;
                rem = best - min * unit;
                sec = rem / per;
                hund = (rem - sec * per) * 100 / per;
                sprintf(line, D_0015F588, get_help_message_text(0x50A3));
                font_print_center(0x100, freeze_stats_text + 0x26, matched_record_text_color, line, -1);
                sprintf(line, freeze_fmt_time, min, sec, hund);
                font_print_center(0x100, freeze_stats_text + 0x3A, matched_record_text_color, line, -1);
            } else {
                sprintf(line, D_0015F588, get_help_message_text(0x50A2));
                font_print_center(0x100, freeze_stats_text + 0x30, unmatched_record_text_color, line, -1);
            }
            record = freeze_stats_score_ref_a[(current_level_index ^ 0x10) ? 0 : 1];
            if (record != 0 && record == hero.race_score) {
                sprintf(line, D_0015F588, get_help_message_text(0x50A5));
                font_print_center(0x100, freeze_stats_text + 0x56, matched_record_text_color, line, -1);
                sprintf(line, freeze_fmt_score, hero.race_score);
                font_print_center(0x100, freeze_stats_text + 0x6A, matched_record_text_color, line, -1);
            } else {
                sprintf(line, D_0015F588, get_help_message_text(0x50A4));
                font_print_center(0x100, freeze_stats_text + 0x60, unmatched_record_text_color, line, -1);
            }
            font_print_center(0x100, freeze_stats_text + 0x90, shared_dialog_text_color, get_help_message_text(0x5249), -1);
            font_print_center(0x100, freeze_stats_text + 0xA8, shared_dialog_text_color, get_help_message_text(0x5248), -1);
        }
        break;
    }
    case FREEZE_TITLE_DIALOG:
        draw_outlined_rect(0x64, 0xA0, 0xB0, 0x150,
                     fast_tween_color(freeze_outline_pulse_base_color, freeze_outline_pulse_blend_target_color,
                                  fast_sin((f32)(game_frame_counter % freeze_wave_period) / convert_integer_to_float(freeze_wave_period) * 6.28318f -
                                               3.14159f) *
                                          0.5f +
                                      0.5f));
        font_print_center(0x100, 0x7A, 0x8000C0C0, freeze.title, -1);
        break;
    default: {
        MessageBox *mb = &freeze;
        s32 pulse;

        pulse = fast_tween_color(freeze_outline_pulse_base_color, freeze_outline_pulse_blend_target_color,
                                  fast_sin((f32)(game_frame_counter % freeze_wave_period) / convert_integer_to_float(freeze_wave_period) * 6.28318f -
                                               3.14159f) *
                                          0.5f +
                                      0.5f);
        draw_outlined_rect(0x50, freeze_ans1 + 0x1A, 0xB0, 0x150, pulse);
        if (mb->title != NULL) {
            font_print_center(0x100, 0x5A, 0x8000C0C0, mb->title, -1);
        }
        if (mb->line1 != NULL) {
            font_print_center(0x100, freeze_ans2, 0x80FFA888, mb->line1, -1);
        }
        if (mb->line2 != NULL) {
            font_print_center(0x100, freeze_ans1, 0x80FFA888, mb->line2, -1);
        }
        break;
    }
    }
    do_gif_paging();
}

extern __typeof__(draw_dialog_text) func_001FBC50 __attribute__((alias("FUN_001fbc50")));
