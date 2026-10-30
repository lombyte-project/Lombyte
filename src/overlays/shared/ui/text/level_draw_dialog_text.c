/* Text: the level overlay's own copy of draw_dialog_text. */
#include "types.h"
#include "sda.h"
#include "rnc/globals.h"
#include "rnc/rendering/screen.h"
#include "rnc/ui/text/text_region.h"
#include "rnc/hero/hero.h"
#include "rnc/data_labels.h"
#include "rnc/game/game_globals.h"
#include "rnc/overlay/level_labels.h"
#include "rnc/rendering/vu1_registers.h"
#include "rnc/rendering/texture/do_gif_paging.h"
#include "rnc/rendering/texture/setup_gif_paging.h"
#include "rnc/ui/hud/get_icon_frame.h"
#include "rnc/ui/help/get_help_message_text.h"
#include "rnc/ui/hud/sprites/draw_hud_sprite.h"
#include "rnc/ui/frames/draw_ui_frame.h"
#include "rnc/ui/text/draw_outlined_rect.h"

/* State of the level's message box (D_L00_00173040). */
typedef struct {
    s32   state; /* 0x00 */
    s32   timer; /* 0x04: fade-in frames */
    char *title; /* 0x08 */
    char *line1; /* 0x0C */
    char *line2; /* 0x10 */
    u8    pad14[8];
    s32   choice; /* 0x1C */
    s32   unk20;  /* 0x20 */
    s32   unk24;  /* 0x24 */
} MessageBox;

extern MessageBox l00_message_box __asm__("D_L00_00173040");

#include "rnc/overlay/shared/ui/text/level_draw_dialog_text_globals.h"
#include "rnc/math/fast_math.h"
#include "rnc/math/asm_helpers.h"

extern char              freeze_dialog_default_body_text[] __asm__("D_L00_0015F508");
extern char              freeze_dialog_body_format[] __asm__("D_L00_0015F510");
extern char              results_indented_label_format[] __asm__("D_L00_0015F520");
extern char              results_first_place_format[] __asm__("D_L00_0015F528");
extern char              results_second_place_format[] __asm__("D_L00_0015F530");
extern char              results_third_place_format[] __asm__("D_L00_0015F538");
extern char              results_other_place_format[] __asm__("D_L00_0015F540");
extern char              results_record_label_format[] __asm__("D_L00_0015F548");
extern char              results_record_time_format[] __asm__("D_L00_0015F550");
extern char              results_record_score_format[] __asm__("D_L00_0015F560");
extern u8                dialog_font_glyph_metrics[] __asm__("D_L00_001C3150");
extern struct TextRegion l00_split_dialog_text_region_template __asm__("D_L00_001E7A50");
extern struct TextRegion l00_choice_dialog_text_region_template __asm__("D_L00_001E7A68");

extern void memset(void *, s32, s32) __asm__("FUN_001153fc");
extern s32  sprintf(char *, const char *, ...) __asm__("FUN_00116248");
extern void strncpy(void *, char *, s32) __asm__("FUN_00116808");
extern s32  get_effect_texture(s32) __asm__("FUN_001f44b8");
extern s32  font_print_center(s32, s32, s64, char *, s32) __asm__("FUN_001f6af0");
extern void
font_print_window(struct TextRegion *, s64, void *, s32, s32, u8 *) __asm__("FUN_001f7090");
extern void
           font_print_window_regular(struct TextRegion *, s32, char *, s32) __asm__("FUN_001f7580");
extern s32 fast_tween_color(s32, s32, f32) __asm__("FUN_001fa6e0");
extern s64 get_frame_texture(s32) __asm__("FUN_001ffa10");
/* Same registers as (w, h, tex, x, y, cx, cy, angle); floats are listed first so they are evaluated first. */
extern void draw_rotated_sprite(
    f32 x, f32 y, f32 cx, f32 cy, f32 angle, s32 w, s32 h, s64 tex) __asm__("FUN_00200600");

#define SCRATCHPAD ((u8 *)0x70000000)

void level_draw_dialog_text(void) __asm__("FUN_L00_002018c8");

void level_draw_dialog_text(void) {
    enum {
        MESSAGE_BOX_RESULTS_PANEL = 0,
        MESSAGE_BOX_TITLE_ONLY = 2,
        MESSAGE_BOX_FREEZE_MODE_DIALOG = 3,
        MESSAGE_BOX_SPLIT_TEXT_SPRITE_DIALOG = 5,
        MESSAGE_BOX_CHOICE_DIALOG = 6
    };
    enum {
        CHOICE_DIALOG_PAIRED_BUTTONS = 0,
        CHOICE_DIALOG_SHARED_PAIRED_BUTTONS = 1,
        CHOICE_DIALOG_ALTERNATE_BODY_PAIRED_BUTTONS = 2,
        CHOICE_DIALOG_SINGLE_CENTERED_BUTTON = 3
    };
    enum {
        FREEZE_DIALOG_TIMER_GATED_CENTERED_BUTTON = 2,
        FREEZE_DIALOG_CONDITIONAL_PAIRED_BUTTONS = 3,
        FREEZE_DIALOG_SHARED_CONDITIONAL_PAIRED_BUTTONS = 4,
        FREEZE_DIALOG_FORMATTED_BODY_CENTERED_BUTTON = 5,
        FREEZE_DIALOG_PLAIN_BODY_ONLY = 7,
        FREEZE_DIALOG_SHARED_PLAIN_BODY_ONLY = 8,
        FREEZE_DIALOG_BODY_ONLY = 10,
        FREEZE_DIALOG_SHARED_BODY_ONLY = 11,
        FREEZE_DIALOG_FLAG_GATED_PAIRED_BUTTONS = 12,
        FREEZE_DIALOG_UNCONDITIONAL_PAIRED_BUTTONS = 13,
        FREEZE_DIALOG_PLAIN_BODY_CENTERED_BUTTON = 17,
        FREEZE_DIALOG_CONDITIONAL_FORMATTED_BODY = 19,
        FREEZE_DIALOG_PLAIN_BODY_PAIRED_BUTTONS = 23,
        FREEZE_DIALOG_FORMATTED_BODY_PAIRED_BUTTONS = 24
    };
    char              formatted_dialog_body[0x200];
    struct TextRegion box;

    setup_gif_paging(0);
    switch (l00_message_box.state) {
    case MESSAGE_BOX_SPLIT_TEXT_SPRITE_DIALOG: {
        u8 *text_cursor;
        s32 color;
        s32 tint;
        s32 y;
        s32 top;
        s32 mid;
        f32 alpha;
        f32 rotated_sprite_y;
        f32 angle;
        f32 pivot;

        alpha = 1.0f - (f32)l00_message_box.timer / (f32)scale_game_frames(30);
        draw_ui_frame(0x50, 0x154, 0x60, 0x1A0, (s32)(alpha * 80.0f));
        box = l00_split_dialog_text_region_template;
        color = fast_tween_color(dialog_text_tween_start_color, dialog_text_tween_end_color, alpha);
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
        font_print_window(
            &box, color, SCRATCHPAD, -1, get_effect_texture(1), dialog_font_glyph_metrics);
        pivot = 272.0f;
        box.flags |= 4;
        y = box.anchor_y;
        y += box.rendered_height;
        font_print_window(
            &box, color, text_cursor, -1, get_effect_texture(1), dialog_font_glyph_metrics);
        box.flags ^= 4;
        box.anchor_y = 0x136 - box.rendered_height;
        font_print_window(
            &box, color, text_cursor, -1, get_effect_texture(1), dialog_font_glyph_metrics);
        tint = fast_tween_color(
            0x20FFFF, 0x8020FFFF, 1.0f - (f32)l00_message_box.unk24 / (f32)scale_game_frames(30));
        font_print_center(0x100, 0x140, tint, get_help_message_text(0x524A), -1);
        mid = (y + box.anchor_y) >> 1;
        vu1_add_g_sregister(0x47, 0x3004B);
        top = mid - 0x20;
        draw_hud_sprite(get_icon_frame(0x755D, 0), 0xE0, top, 0x40, 0x40, 0x80);
        rotated_sprite_y = (f32)(mid << 4);
        angle = (f32)(D_L00_0015F3F8 % 55) * -6.2831855f / 55.0f;
        draw_rotated_sprite(4096.0f,
                            rotated_sprite_y,
                            pivot,
                            pivot,
                            angle,
                            0x40,
                            0x40,
                            get_frame_texture(get_icon_frame(0x755D, 1)));
        break;
    }
    case MESSAGE_BOX_FREEZE_MODE_DIALOG: {
        char *text;
        s32   left;
        s32   right;
        s32   mid;
        s32   tint;
        s32   y;
        s32   h;
        f32   alpha;

        text = freeze_dialog_default_body_text;
        left = 0;
        mid = 0;
        right = 0;
        switch (mode_freeze_state) {
        case 6:
            left = 0x524F;
            right = 0x524B;
            text = get_help_message_text(0x4FAF);
            break;
        case FREEZE_DIALOG_FLAG_GATED_PAIRED_BUTTONS:
            if (!(mode_freeze_flags & 2)) {
                mid = 0x4FA8;
                text = get_help_message_text(0x4FA7);
                break;
            }
        case FREEZE_DIALOG_UNCONDITIONAL_PAIRED_BUTTONS:
            left = 0x524F;
            right = 0x524B;
            text = get_help_message_text(0x4FAD);
            break;
        case FREEZE_DIALOG_BODY_ONLY:
        case FREEZE_DIALOG_SHARED_BODY_ONLY:
            text = get_help_message_text(0x4FC1);
            break;
        case FREEZE_DIALOG_PLAIN_BODY_ONLY:
        case FREEZE_DIALOG_SHARED_PLAIN_BODY_ONLY:
            text = get_help_message_text(0x4FB7);
            break;
        case 14:
        case 15:
            text = get_help_message_text(0x4FB8);
            break;
        case FREEZE_DIALOG_PLAIN_BODY_CENTERED_BUTTON:
            mid = 0x524A;
            text = get_help_message_text(0x4FBA);
            break;
        case 18:
            mid = 0x524A;
            text = get_help_message_text(0x4FBC);
            break;
        case FREEZE_DIALOG_TIMER_GATED_CENTERED_BUTTON:
            mid = 0x524A;
            text = get_help_message_text(0x4FA6);
            if (l00_message_box.timer != 0) {
                mid = 0;
            }
            break;
        case FREEZE_DIALOG_CONDITIONAL_FORMATTED_BODY:
            if (mode_freeze_flags & 4) {
                mid = 0x4FA8;
                text = get_help_message_text(0x4FA7);
            } else if (D_L00_0015F5A8 != 0) {
                left = 0x524E;
                right = 0x524B;
                sprintf(formatted_dialog_body,
                        freeze_dialog_body_format,
                        get_help_message_text(0x4FA9),
                        1,
                        1,
                        get_help_message_text(0x4FAA));
                text = formatted_dialog_body;
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
        case FREEZE_DIALOG_FORMATTED_BODY_PAIRED_BUTTONS:
            left = 0x524E;
            right = 0x524B;
            sprintf(formatted_dialog_body,
                    freeze_dialog_body_format,
                    get_help_message_text(0x4FAE),
                    1,
                    1,
                    get_help_message_text(0x4FAA));
            text = formatted_dialog_body;
            break;
        case FREEZE_DIALOG_PLAIN_BODY_PAIRED_BUTTONS:
            left = 0x524E;
            right = 0x524B;
            text = get_help_message_text(0x4FB1);
            break;
        case FREEZE_DIALOG_CONDITIONAL_PAIRED_BUTTONS:
        case FREEZE_DIALOG_SHARED_CONDITIONAL_PAIRED_BUTTONS:
            if (D_L00_0015F5A8 != 0 && (mode_freeze_flags & 2)) {
                left = 0x524E;
                right = 0x524B;
                text = get_help_message_text(0x4FAB);
            } else {
                mid = 0x4FA8;
                text = get_help_message_text(0x4FAC);
            }
            break;
        case FREEZE_DIALOG_FORMATTED_BODY_CENTERED_BUTTON:
            mid = 0x4FA8;
            sprintf(formatted_dialog_body,
                    freeze_dialog_body_format,
                    get_help_message_text(0x4FB0),
                    1,
                    1,
                    get_help_message_text(0x4FA7));
            text = formatted_dialog_body;
            break;
        }
        box = (struct TextRegion){0, screen_extent.height, 0x60, 0x1A0, 0x100, 0x68, 0, 0, 0x10, 5};
        font_print_window_regular(&box, 0, text, -1);
        h = box.rendered_height + 0x28;
        y = (screen_extent.height - h) >> 1;
        box.bottom = y + h;
        box.anchor_y = y + 4;
        box.top = y;
        alpha = 1.0f - (f32)l00_message_box.timer / (f32)scale_game_frames(30);
        draw_ui_frame(box.top, box.bottom, 0x60, 0x1A0, (s32)(alpha * 80.0f));
        box.flags ^= 4;
        font_print_window_regular(
            &box,
            fast_tween_color(dialog_text_tween_start_color, dialog_text_tween_end_color, alpha),
            text,
            -1);
        tint = fast_tween_color(
            0x20FFFF, 0x8020FFFF, 1.0f - (f32)l00_message_box.unk24 / (f32)scale_game_frames(30));
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
    case MESSAGE_BOX_CHOICE_DIALOG: {
        s32 text_color;
        s32 button_color;
        s32 id;
        f32 alpha;

        box = l00_choice_dialog_text_region_template;
        alpha = 1.0f - (f32)l00_message_box.timer / (f32)scale_game_frames(30);
        draw_ui_frame(0x64, 0x12C, 0x60, 0x1A0, (s32)(alpha * 80.0f));
        text_color =
            fast_tween_color(dialog_text_tween_start_color, dialog_text_tween_end_color, alpha);
        button_color = fast_tween_color(0x20FFFF, 0x8020FFFF, alpha);
        switch (l00_message_box.choice) {
        case CHOICE_DIALOG_PAIRED_BUTTONS:
        case CHOICE_DIALOG_SHARED_PAIRED_BUTTONS:
            font_print_center(0xCA, 0x118, button_color, get_help_message_text(0x524E), -1);
            font_print_center(0x135, 0x118, button_color, get_help_message_text(0x524B), -1);
            id = 0x522A;
            break;
        case CHOICE_DIALOG_ALTERNATE_BODY_PAIRED_BUTTONS:
            font_print_center(0xCA, 0x118, button_color, get_help_message_text(0x524E), -1);
            font_print_center(0x135, 0x118, button_color, get_help_message_text(0x524B), -1);
            id = 0x522B;
            break;
        case CHOICE_DIALOG_SINGLE_CENTERED_BUTTON:
            font_print_center(0x100, 0x118, button_color, get_help_message_text(0x524A), -1);
            id = 0x522C;
            break;
        default:
            id = 0;
            break;
        }
        if (id != 0) {
            font_print_window(&box,
                              text_color,
                              get_help_message_text(id),
                              -1,
                              get_effect_texture(1),
                              dialog_font_glyph_metrics);
        }
        break;
    }
    case MESSAGE_BOX_RESULTS_PANEL: {
        char line[0x40];
        s32  pulse;
        s32  results_label_color;
        s32  first_place_or_record_color;
        s32  other_place_or_no_record_color;
        s32  best;
        s32  race_score_record;
        s32  unit;
        s32  rem;
        s32  per;
        s32  min;
        s32  sec;
        s32  hund;
        f32  scale;
        f32  color_blend_factor;

        pulse = fast_tween_color(dialog_frame_pulse_start_color,
                                 dialog_frame_pulse_end_color,
                                 fast_sin((f32)(D_L00_0015F3F8 % dialog_frame_pulse_period) /
                                              convert_integer_to_float(dialog_frame_pulse_period) *
                                              6.28318f -
                                          3.14159f) *
                                         0.5f +
                                     0.5f);
        scale = (f32)l00_message_box.unk20 * 0.125f;
        if (scale > 1.0f) {
            scale = 1.0f;
        } else if (scale < 0.1f) {
            scale = 0.1f;
        }
        results_label_color = results_label_tween_start_color;
        first_place_or_record_color = first_place_or_record_tween_start_color;
        other_place_or_no_record_color = other_place_or_no_record_tween_start_color;
        if (l00_message_box.unk24 != 0) {
            color_blend_factor = (f32)l00_message_box.unk24 * 0.125f;
            if (color_blend_factor > 1.0f) {
                color_blend_factor = 1.0f;
            } else if (color_blend_factor < 0.0f) {
                color_blend_factor = 0.0f;
            }
            results_label_color = fast_tween_color(
                results_label_tween_start_color, results_label_tween_end_color, color_blend_factor);
            first_place_or_record_color = fast_tween_color(first_place_or_record_tween_start_color,
                                                           first_place_or_record_tween_end_color,
                                                           color_blend_factor);
            other_place_or_no_record_color =
                fast_tween_color(other_place_or_no_record_tween_start_color,
                                 other_place_or_no_record_tween_end_color,
                                 color_blend_factor);
        }
        if (hero.race_lap < 3) {
            draw_outlined_rect(
                results_fixed_labels_frame_center_y -
                    truncate_float_to_s32((f32)results_fixed_labels_frame_half_height * scale),
                results_fixed_labels_frame_center_y +
                    truncate_float_to_s32((f32)results_fixed_labels_frame_half_height * scale),
                results_fixed_labels_frame_center_x -
                    truncate_float_to_s32((f32)results_fixed_labels_frame_half_width * scale),
                results_fixed_labels_frame_center_x +
                    truncate_float_to_s32((f32)results_fixed_labels_frame_half_width * scale),
                pulse);
            font_print_center(0x100,
                              results_fixed_labels_base_y,
                              results_label_color,
                              get_help_message_text(0x4F6E),
                              -1);
            font_print_center(0x100,
                              results_fixed_labels_base_y + 0x18,
                              results_label_color,
                              get_help_message_text(0x5249),
                              -1);
            font_print_center(0x100,
                              results_fixed_labels_base_y + 0x30,
                              results_label_color,
                              get_help_message_text(0x5248),
                              -1);
        } else {
            draw_outlined_rect(
                results_rank_and_records_frame_center_y -
                    truncate_float_to_s32((f32)results_rank_and_records_frame_half_height * scale),
                results_rank_and_records_frame_center_y +
                    truncate_float_to_s32((f32)results_rank_and_records_frame_half_height * scale),
                results_rank_and_records_frame_center_x -
                    truncate_float_to_s32((f32)results_rank_and_records_frame_half_width * scale),
                results_rank_and_records_frame_center_x +
                    truncate_float_to_s32((f32)results_rank_and_records_frame_half_width * scale),
                pulse);
            sprintf(line, results_indented_label_format, get_help_message_text(0x5240));
            if (hero.race_position == 1) {
                sprintf(line, results_first_place_format, 1);
            } else if (hero.race_position == 2) {
                sprintf(line, results_second_place_format, 2);
            } else if (hero.race_position == 3) {
                sprintf(line, results_third_place_format, 3);
            } else {
                sprintf(line, results_other_place_format, hero.race_position);
            }
            line[3] = ' ';
            if (hero.race_position == 1) {
                font_print_center(
                    0x100, results_rank_and_records_base_y, first_place_or_record_color, line, -1);
            } else {
                font_print_center(0x100,
                                  results_rank_and_records_base_y,
                                  other_place_or_no_record_color,
                                  line,
                                  -1);
            }
            best = freeze_stats_time_ref[(current_level_index ^ 0x10) ? 0 : 1];
            if (best == hero.race_time_frames) {
                unit = pal_mode ? 3000 : 3600;
                per = unit / 60;
                min = best / unit;
                rem = best - min * unit;
                sec = rem / per;
                hund = (rem - sec * per) * 100 / per;
                sprintf(line, results_record_label_format, get_help_message_text(0x50A3));
                font_print_center(0x100,
                                  results_rank_and_records_base_y + 0x26,
                                  first_place_or_record_color,
                                  line,
                                  -1);
                sprintf(line, results_record_time_format, min, sec, hund);
                font_print_center(0x100,
                                  results_rank_and_records_base_y + 0x3A,
                                  first_place_or_record_color,
                                  line,
                                  -1);
            } else {
                sprintf(line, results_record_label_format, get_help_message_text(0x50A2));
                font_print_center(0x100,
                                  results_rank_and_records_base_y + 0x30,
                                  other_place_or_no_record_color,
                                  line,
                                  -1);
            }
            race_score_record = freeze_stats_score_ref_a[(current_level_index ^ 0x10) ? 0 : 1];
            if (race_score_record != 0 && race_score_record == hero.race_score) {
                sprintf(line, results_record_label_format, get_help_message_text(0x50A5));
                font_print_center(0x100,
                                  results_rank_and_records_base_y + 0x56,
                                  first_place_or_record_color,
                                  line,
                                  -1);
                sprintf(line, results_record_score_format, hero.race_score);
                font_print_center(0x100,
                                  results_rank_and_records_base_y + 0x6A,
                                  first_place_or_record_color,
                                  line,
                                  -1);
            } else {
                sprintf(line, results_record_label_format, get_help_message_text(0x50A4));
                font_print_center(0x100,
                                  results_rank_and_records_base_y + 0x60,
                                  other_place_or_no_record_color,
                                  line,
                                  -1);
            }
            font_print_center(0x100,
                              results_rank_and_records_base_y + 0x90,
                              results_label_color,
                              get_help_message_text(0x5249),
                              -1);
            font_print_center(0x100,
                              results_rank_and_records_base_y + 0xA8,
                              results_label_color,
                              get_help_message_text(0x5248),
                              -1);
        }
        break;
    }
    case MESSAGE_BOX_TITLE_ONLY:
        draw_outlined_rect(
            0x64,
            0xA0,
            0xB0,
            0x150,
            fast_tween_color(dialog_frame_pulse_start_color,
                             dialog_frame_pulse_end_color,
                             fast_sin((f32)(D_L00_0015F3F8 % dialog_frame_pulse_period) /
                                          convert_integer_to_float(dialog_frame_pulse_period) *
                                          6.28318f -
                                      3.14159f) *
                                     0.5f +
                                 0.5f));
        font_print_center(0x100, 0x7A, 0x8000C0C0, l00_message_box.title, -1);
        break;
    default: {
        MessageBox *mb = &l00_message_box;
        s32         pulse;

        pulse = fast_tween_color(dialog_frame_pulse_start_color,
                                 dialog_frame_pulse_end_color,
                                 fast_sin((f32)(D_L00_0015F3F8 % dialog_frame_pulse_period) /
                                              convert_integer_to_float(dialog_frame_pulse_period) *
                                              6.28318f -
                                          3.14159f) *
                                         0.5f +
                                     0.5f);
        draw_outlined_rect(0x50, default_dialog_line2_y + 0x1A, 0xB0, 0x150, pulse);
        if (mb->title != NULL) {
            font_print_center(0x100, 0x5A, 0x8000C0C0, mb->title, -1);
        }
        if (mb->line1 != NULL) {
            font_print_center(0x100, default_dialog_line1_y, 0x80FFA888, mb->line1, -1);
        }
        if (mb->line2 != NULL) {
            font_print_center(0x100, default_dialog_line2_y, 0x80FFA888, mb->line2, -1);
        }
        break;
    }
    }
    do_gif_paging();
}

MessageBox D_L01_001734C0 = {0};

struct TextRegion D_L01_00208AB0 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L01_00208AC8 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L02_00173640 = {0};

struct TextRegion D_L02_001F8350 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L02_001F8368 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L03_00173140 = {0};

struct TextRegion D_L03_001E06D0 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L03_001E06E8 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L04_001731C0 = {0};

struct TextRegion D_L04_001D9840 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L04_001D9858 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L05_00173440 = {0};

struct TextRegion D_L05_00216140 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L05_00216158 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L06_001737C0 = {0};

struct TextRegion D_L06_001FEEA0 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L06_001FEEB8 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L07_001730C0 = {0};

struct TextRegion D_L07_0020ED40 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L07_0020ED58 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L08_00173840 = {0};

struct TextRegion D_L08_001F9990 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L08_001F99A8 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L09_001731C0 = {0};

struct TextRegion D_L09_002065B0 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L09_002065C8 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L10_001734C0 = {0};

struct TextRegion D_L10_001DA820 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L10_001DA838 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L11_00173A40 = {0};

struct TextRegion D_L11_00218BA0 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L11_00218BB8 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L12_001734C0 = {0};

struct TextRegion D_L12_00209F30 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L12_00209F48 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L13_00173340 = {0};

struct TextRegion D_L13_001F2510 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L13_001F2528 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L14_00173740 = {0};

struct TextRegion D_L14_001F9DA0 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L14_001F9DB8 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L15_00173640 = {0};

struct TextRegion D_L15_001E0610 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L15_001E0628 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L16_00173440 = {0};

struct TextRegion D_L16_001E5D00 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L16_001E5D18 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L17_00173940 = {0};

struct TextRegion D_L17_001E4460 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L17_001E4478 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

MessageBox D_L18_00173A40 = {0};

struct TextRegion D_L18_001F0180 = {0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion D_L18_001F0198 = {0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};

struct TextRegion l00_split_dialog_text_region_template __asm__("D_L00_001E7A50") = {
    0x50, 0x154, 0x60, 0x1a0, 0x100, 0x54, 0, 0, 0x10, 1, 0, 0};

struct TextRegion l00_choice_dialog_text_region_template __asm__("D_L00_001E7A68") = {
    0x64, 0x12c, 0x60, 0x1a0, 0x100, 0x68, 0, 0, 0x10, 1, 0, 0};
