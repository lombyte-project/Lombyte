#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/draw_dialog_text/FUN_001fbc50.s", FUN_001fbc50);
#else
#include "rnc/rendering_draw_dialog_text_types.h"
#include "types.h"

extern u8 D_0013E500[];
extern struct DialogGameState D_0013F350;
extern s32 D_0015ED80[];
extern s32 D_0015ED84 __attribute__((sda));
extern u8 D_0015EE58[];
extern u8 D_0015EE68[];
extern s32 D_0015EEB0[];
extern s32 D_0015EEB4[];
extern s32 D_0015F438[];
extern s32 D_0015F4E8 __attribute__((sda));
extern s32 D_0015F4EC __attribute__((sda));
extern s32 D_0015F4F0 __attribute__((sda));
extern s32 D_0015F4F4 __attribute__((sda));
extern s32 D_0015F4F8 __attribute__((sda));
extern s32 D_0015F4FC __attribute__((sda));
extern f32 D_0015F500 __attribute__((sda));
extern f32 D_0015F504 __attribute__((sda));
extern s32 D_0015F508 __attribute__((sda));
extern s32 D_0015F50C __attribute__((sda));
extern s32 D_0015F510 __attribute__((sda));
extern f32 D_0015F514 __attribute__((sda));
extern f32 D_0015F518 __attribute__((sda));
extern s32 D_0015F51C __attribute__((sda));
extern s32 D_0015F520 __attribute__((sda));
extern s32 D_0015F524 __attribute__((sda));
extern s32 D_0015F528 __attribute__((sda));
extern s32 D_0015F52C __attribute__((sda));
extern s32 D_0015F530 __attribute__((sda));
extern s32 D_0015F534 __attribute__((sda));
extern s32 D_0015F538 __attribute__((sda));
extern s32 D_0015F53C __attribute__((sda));
extern s32 D_0015F540 __attribute__((sda));
extern u8 D_0015F548[];
extern u8 D_0015F550[];
extern u8 D_0015F560[];
extern u8 D_0015F568[];
extern u8 D_0015F570[];
extern u8 D_0015F578[];
extern u8 D_0015F580[];
extern u8 D_0015F588[];
extern u8 D_0015F590[];
extern u8 D_0015F5A0[];
extern s32 D_0015F5E8[];
extern struct ModalScreenState D_00193300;
extern u8 D_001DF050[];
extern u8 D_001E78F0[];
extern u8 D_001E7908[];
extern void func_001F4280(s32);
extern void func_001F4398(void);
extern u8 *func_001FDD10(s32);
extern s32 func_001F96F8(s32);
extern void func_001F5F18(s32, s32, s32, s32, s32);
extern s32 func_001FA6E0(s32, s32, f32);
extern void func_001F6AF0(s32, s32, s64, u8 *, s32);
extern void func_001F7090(struct TextRegion *, s64, u8 *, s32, s32, u8 *);
extern void func_001F7580(struct TextRegion *, s64, u8 *, s32);
extern void func_001F6060(s32, s32, s32, s32, s32);
extern s32 func_001F44B8(s32);
extern f32 func_001F9DE0(f32);
extern f32 func_001FA6C0(s32);
extern s32 func_001FA6D0(f32);
extern s32 func_001FF960(s32, s32);
extern s32 func_001FFA10(s32);
extern void func_001FFC30(s32, s32, s32, s32, s32, s32);
extern void func_00200600(s32, s32, s32, f32, f32, f32, f32, f32);
extern void func_00233980(s32, s32);
extern void *memset(void *, s32, u32);
extern s32 sprintf(u8 *, u8 *, ...);
extern u8 *strncpy(u8 *, const u8 *, u32);

/* Draw the current modal screen, including timed text, hints and statistics. */
void draw_dialog_text(void) __asm__("FUN_001fbc50");

void draw_dialog_text(void) {
    struct DialogTextFrame frame;
    f32 sprite_extent;
    f32 transition_progress;
    f32 phase_progress;
    f32 transition_duration;
    f32 text_fade;
    f32 phase_fade;
    f32 pulse_period;
    f32 message_pulse_period;
    f32 default_pulse_period;
    f32 colour_fade;
    f32 panel_scale;
    s16 panel_height;
    s32 dialog_colour;
    s32 continue_colour;
    s32 hint_colour;
    s32 panel_top;
    s32 panel_bottom;
    s32 panel_left;
    s32 alternate_top;
    s32 alternate_bottom;
    s32 alternate_left;
    s32 panel_colour;
    s32 panel_y;
    s32 colour_timer;
    s32 *record_time;
    u8 *display_state;
    s32 record_time_offset;
    s32 frame_rate_scale;
    s32 record_count_offset;
    s32 record_frames;
    s32 frames_per_minute;
    s32 minutes;
    s32 frames_per_second;
    s32 seconds;
    s32 hundredths;
    s32 record_count;
    s32 rotation_frame;
    s32 pulse_frame;
    s32 message_pulse_frame;
    s32 default_pulse_frame;
    s32 first_text_bottom;
    s32 icon_y;
    s32 phase_text_colour;
    s32 phase_hint_colour;
    s32 phase;
    u8 *phase_text;
    s32 center_hint_id;
    s32 right_hint_id;
    s32 left_hint_id;
    s32 primary_stat_colour;
    s32 icon_top;
    s32 secondary_stat_colour;
    s32 choice_text_id;
    s32 message_text_id;
    s32 suffix_text_id;
    s32 phase_text_id;
    s32 final_text_y;
    s32 final_text_colour;
    u8 *message_format;
    u8 *message_text;
    u8 *message_prefix;
    u8 *split_text;
    u8 *final_text;

    func_001F4280(0);
    switch (D_00193300.mode) {
    case 5:
        transition_progress = (f32) D_00193300.countdown / (f32) func_001F96F8(0x1E);
        func_001F5F18(0x50, 0x154, 0x60, 0x1A0, (s32) ((1.0f - transition_progress) * 80.0f));
        *(struct TextRegionBytes *) &frame.region = *(struct TextRegionBytes *) D_001E78F0;
        dialog_colour = func_001FA6E0(D_0015F4F0, D_0015F4F4, 1.0f - transition_progress);
        strncpy((u8 *) 0x70000000, func_001FDD10(0x4E2B), 0x400);
        split_text = (u8 *) 0x70000000;
        while (*split_text >= 2U) {
            split_text += 1;
        }
        while (*split_text == 1) {
            *split_text = 0;
            split_text += 1;
        }
        func_001F7090(&frame.region, dialog_colour, (u8 *) 0x70000000, -1, func_001F44B8(1), D_001DF050);
        sprite_extent = 272.0f;
        frame.region.flags |= TEXT_REGION_MEASURE_ONLY;
        first_text_bottom = frame.region.anchor_y + frame.region.rendered_height;
        func_001F7090(&frame.region, dialog_colour, split_text, -1, func_001F44B8(1), D_001DF050);
        frame.region.flags ^= TEXT_REGION_MEASURE_ONLY;
        frame.region.anchor_y = 0x136 - (u16) frame.region.rendered_height;
        func_001F7090(&frame.region, dialog_colour, split_text, -1, func_001F44B8(1), D_001DF050);
        continue_colour = func_001FA6E0(0x20FFFF, 0x8020FFFF, 1.0f - ((f32) D_00193300.secondary_countdown / (f32) func_001F96F8(0x1E)));
        func_001F6AF0(0x100, 0x140, continue_colour, func_001FDD10(0x524A), -1);
        icon_y = (s32) (first_text_bottom + frame.region.anchor_y) >> 1;
        func_00233980(0x47, 0x3004B);
        icon_top = icon_y - 0x20;
        func_001FFC30(func_001FF960(0x755D, 0), 0xE0, icon_top, 0x40, 0x40, 0x80);
        rotation_frame = (s32) D_0015F438[0] % 55;
        func_00200600(0x40, 0x40, func_001FFA10(func_001FF960(0x755D, 1)), 4096.0f, (f32) (icon_y * 0x10), sprite_extent, sprite_extent, ((f32) rotation_frame * -6.2831855f) / 55.0f);
        break;
    case 3:
        message_text = D_0015F548;
        left_hint_id = 0;
        center_hint_id = 0;
        right_hint_id = 0;
        switch (D_0015EEB0[0]) {
        case 6:
            choice_text_id = 0x4FAF;
            left_hint_id = 0x524F;
            goto resolve_choice_text;
        case 12:
            message_text_id = 0x4FA7;
            if (D_0015EEB4[0] & 2) {
            case 13:
                choice_text_id = 0x4FAD;
                left_hint_id = 0x524F;
                goto resolve_choice_text;
            }
            goto set_center_hint;
        case 10:
        case 11:
            message_text_id = 0x4FC1;
            goto resolve_message_text;
        case 7:
        case 8:
            message_text_id = 0x4FB7;
            goto resolve_message_text;
        case 14:
        case 15:
            message_text_id = 0x4FB8;
            goto resolve_message_text;
        case 17:
            message_text_id = 0x4FBA;
            center_hint_id = 0x524A;
            goto resolve_message_text;
        case 18:
            message_text_id = 0x4FBC;
            center_hint_id = 0x524A;
            goto resolve_message_text;
        case 2:
            message_text = func_001FDD10(0x4FA6);
            center_hint_id = (D_00193300.countdown != 0) ? 0 : 0x524A;
            break;
        case 19:
            message_text_id = 0x4FA7;
            if (D_0015EEB4[0] & 4) {
                goto set_center_hint;
            }
            if (D_0015F5E8[0] == 0) {
                message_text_id = 0x4FA9;
                goto set_center_hint;
            }
            left_hint_id = 0x524E;
            right_hint_id = 0x524B;
            message_format = D_0015F550;
            message_prefix = func_001FDD10(0x4FA9);
            suffix_text_id = 0x4FAA;
            goto format_message_text;
        case 21:
            message_text_id = 0x4FBD;
            center_hint_id = 0x524A;
            goto resolve_message_text;
        case 20:
            message_text_id = 0x4FBB;
            center_hint_id = 0x524A;
            goto resolve_message_text;
        case 24:
            left_hint_id = 0x524E;
            right_hint_id = 0x524B;
            message_format = D_0015F550;
            message_prefix = func_001FDD10(0x4FAE);
            suffix_text_id = 0x4FAA;
            goto format_message_text;
        case 23:
            choice_text_id = 0x4FB1;
            left_hint_id = 0x524E;
            goto resolve_choice_text;
        case 3:
        case 4:
            if ((D_0015F5E8[0] != 0) && (D_0015EEB4[0] & 2)) {
                choice_text_id = 0x4FAB;
                left_hint_id = 0x524E;
resolve_choice_text:
                right_hint_id = 0x524B;
                message_text = func_001FDD10(choice_text_id);
                break;
            }
            message_text_id = 0x4FAC;
set_center_hint:
            center_hint_id = 0x4FA8;
resolve_message_text:
            message_text = func_001FDD10(message_text_id);
            break;
        case 5:
            center_hint_id = 0x4FA8;
            message_format = D_0015F550;
            message_prefix = func_001FDD10(0x4FB0);
            suffix_text_id = 0x4FA7;
format_message_text:
            sprintf(frame.text, message_format, message_prefix, 1, 1, func_001FDD10(suffix_text_id));
            message_text = frame.text;
            break;
        }
        memset(&frame.input_region, 0, 0x18);
        display_state = D_0013E500;
        frame.input_region.bottom = *(u16 *) (display_state + 0x4);
        frame.input_region.left = 0x60;
        frame.input_region.right = 0x1A0;
        frame.input_region.anchor_x = 0x100;
        frame.input_region.anchor_y = 0x68;
        frame.input_region.line_advance = 0x10;
        frame.input_region.flags = TEXT_REGION_CENTER_HORIZONTALLY | TEXT_REGION_MEASURE_ONLY;
        *(struct TextRegionBytes *) &frame.region = *(struct TextRegionBytes *) &frame.input_region;
        func_001F7580(&frame.region, 0, message_text, -1);
        panel_height = (frame.region.rendered_height + 0x28);
        panel_y = (s32) (*(s32 *) (display_state + 0x4) - panel_height) >> 1;
        frame.region.anchor_y = panel_y + 4;
        frame.region.bottom = panel_y + panel_height;
        frame.region.top = (s16) panel_y;
        transition_duration = (f32) func_001F96F8(0x1E);
        text_fade = 1.0f - ((f32) D_00193300.countdown / transition_duration);
        func_001F5F18(frame.region.top, frame.region.bottom, 0x60, 0x1A0, (s32) (text_fade * 80.0f));
        frame.region.flags ^= TEXT_REGION_MEASURE_ONLY;
        func_001F7580(&frame.region, func_001FA6E0(D_0015F4F0, D_0015F4F4, text_fade), message_text, -1);
        hint_colour = func_001FA6E0(0x20FFFF, 0x8020FFFF, 1.0f - ((f32) D_00193300.secondary_countdown / (f32) func_001F96F8(0x1E)));
        if (left_hint_id != 0) {
            func_001F6AF0(0xCA, frame.region.bottom - 0x14, hint_colour, func_001FDD10(left_hint_id), -1);
        }
        if (right_hint_id != 0) {
            func_001F6AF0(0x135, frame.region.bottom - 0x14, hint_colour, func_001FDD10(right_hint_id), -1);
        }
        if (center_hint_id != 0) {
            final_text_y = frame.region.bottom - 0x14;
            final_text_colour = hint_colour;
            final_text = func_001FDD10(center_hint_id);
            goto draw_final_text;
        }
        break;
    case 6:
        *(struct TextRegionBytes *) &frame.region = *(struct TextRegionBytes *) D_001E7908;
        phase_progress = (f32) D_00193300.countdown / (f32) func_001F96F8(0x1E);
        phase_fade = 1.0f - phase_progress;
        func_001F5F18(0x64, 0x12C, 0x60, 0x1A0, (s32) (phase_fade * 80.0f));
        phase_text_colour = func_001FA6E0(D_0015F4F0, D_0015F4F4, phase_fade);
        phase_hint_colour = func_001FA6E0(0x20FFFF, 0x8020FFFF, phase_fade);
        phase = D_00193300.phase;
        if (phase != 2) {
            if (phase >= 3) {
                if (phase != 3) {
                    goto no_phase_text;
                }
                func_001F6AF0(0x100, 0x118, phase_hint_colour, func_001FDD10(0x524A), -1);
                phase_text_id = 0x522C;
            } else if (phase >= 0) {
                func_001F6AF0(0xCA, 0x118, phase_hint_colour, func_001FDD10(0x524E), -1);
                func_001F6AF0(0x135, 0x118, phase_hint_colour, func_001FDD10(0x524B), -1);
                phase_text_id = 0x522A;
            } else {
no_phase_text:
                phase_text_id = 0;
            }
        } else {
            func_001F6AF0(0xCA, 0x118, phase_hint_colour, func_001FDD10(0x524E), -1);
            func_001F6AF0(0x135, 0x118, phase_hint_colour, func_001FDD10(0x524B), -1);
            phase_text_id = 0x522B;
        }
        if (phase_text_id != 0) {
            phase_text = func_001FDD10(phase_text_id);
            func_001F7090(&frame.region, phase_text_colour, phase_text, -1, func_001F44B8(1), D_001DF050);
        }
        break;
    case 0:
        pulse_period = func_001FA6C0(D_0015F520);
        pulse_frame = (s32) D_0015F438[0] % (s32) D_0015F520;
        panel_colour = func_001FA6E0(D_0015F524, D_0015F528, (func_001F9DE0((((f32) pulse_frame / pulse_period) * 6.28318f) - 3.14159f) * 0.5f) + 0.5f);
        panel_scale = (f32) D_00193300.elapsed_frames * 0.125f;
        if (panel_scale > 1.0f) {
            panel_scale = 1.0f;
        } else if (panel_scale < 0.1f) {
            panel_scale = 0.1f;
        }
        frame.text_colour = D_0015F52C;
        primary_stat_colour = D_0015F534;
        colour_timer = D_00193300.secondary_countdown;
        secondary_stat_colour = D_0015F53C;
        if (colour_timer != 0) {
            colour_fade = (f32) colour_timer * 0.125f;
            if (colour_fade > 1.0f) {
                colour_fade = 1.0f;
            } else if (colour_fade < 0.0f) {
                colour_fade = 0.0f;
            }
            frame.text_colour = func_001FA6E0(D_0015F52C, D_0015F530, colour_fade);
            primary_stat_colour = func_001FA6E0(D_0015F534, D_0015F538, colour_fade);
            secondary_stat_colour = func_001FA6E0(D_0015F53C, D_0015F540, colour_fade);
        }
        if (D_0013F350.panel_variant < 3) {
            panel_top = D_0015F4FC - func_001FA6D0((f32) D_0015F504 * panel_scale);
            panel_bottom = D_0015F4FC + func_001FA6D0(D_0015F504 * panel_scale);
            panel_left = D_0015F4F8 - func_001FA6D0((f32) D_0015F500 * panel_scale);
            func_001F6060(panel_top, panel_bottom, panel_left, D_0015F4F8 + func_001FA6D0((f32) D_0015F500 * panel_scale), panel_colour);
            func_001F6AF0(0x100, D_0015F508, frame.text_colour, func_001FDD10(0x4F6E), -1);
            func_001F6AF0(0x100, D_0015F508 + 0x18, frame.text_colour, func_001FDD10(0x5249), -1);
            final_text_y = D_0015F508 + 0x30;
            final_text_colour = frame.text_colour;
            final_text = func_001FDD10(0x5248);
        } else {
            alternate_top = D_0015F510 - func_001FA6D0((f32) D_0015F518 * panel_scale);
            alternate_bottom = D_0015F510 + func_001FA6D0((f32) D_0015F518 * panel_scale);
            alternate_left = D_0015F50C - func_001FA6D0((f32) D_0015F514 * panel_scale);
            func_001F6060(alternate_top, alternate_bottom, alternate_left, D_0015F50C + func_001FA6D0((f32) D_0015F514 * panel_scale), panel_colour);
            sprintf(frame.statistics_text, D_0015F560, func_001FDD10(0x5240));
            if (D_0013F350.display_value == 1) {
                sprintf(frame.statistics_text, D_0015F568, 1);
            } else if (D_0013F350.display_value == 2) {
                sprintf(frame.statistics_text, D_0015F570, 2);
            } else if (D_0013F350.display_value == 3) {
                sprintf(frame.statistics_text, D_0015F578, 3);
            } else {
                sprintf(frame.statistics_text, D_0015F580, D_0013F350.display_value);
            }
            frame.statistics_text[3] = 0x20;
            if (D_0013F350.display_value == 1) {
                func_001F6AF0(0x100, D_0015F51C, primary_stat_colour, frame.statistics_text, -1);
            } else {
                func_001F6AF0(0x100, D_0015F51C, secondary_stat_colour, frame.statistics_text, -1);
            }
            if ((D_0015ED84 ^ 0x10) != 0) {
                record_time_offset = 0;
            } else {
                record_time_offset = 4;
            }
            record_time = (s32 *) (record_time_offset + D_0015EE58);
            record_frames = *record_time;
            if (record_frames == D_0013F350.time_value) {
                if (D_0015ED80[0] != 0) {
                    frame_rate_scale = 0xBB8;
                } else {
                    frame_rate_scale = 0xE10;
                }
                frames_per_minute = frame_rate_scale;
                minutes = record_frames / frames_per_minute;
                frames_per_second = frames_per_minute / 60;
                seconds = (s32) (*record_time % frames_per_minute) / frames_per_second;
                hundredths = (s32) (((*record_time % frames_per_minute) % frames_per_second) * 0x64) / frames_per_second;
                sprintf(frame.statistics_text, D_0015F588, func_001FDD10(0x50A3));
                func_001F6AF0(0x100, D_0015F51C + 0x26, primary_stat_colour, frame.statistics_text, -1);
                sprintf(frame.statistics_text, D_0015F590, minutes, seconds, hundredths);
                func_001F6AF0(0x100, D_0015F51C + 0x3A, primary_stat_colour, frame.statistics_text, -1);
            } else {
                sprintf(frame.statistics_text, D_0015F588, func_001FDD10(0x50A2));
                func_001F6AF0(0x100, D_0015F51C + 0x30, secondary_stat_colour, frame.statistics_text, -1);
            }
            if ((D_0015ED84 ^ 0x10) != 0) {
                record_count_offset = 0;
            } else {
                record_count_offset = 4;
            }
            record_count = *(s32 *) (record_count_offset + D_0015EE68);
            if (record_count != 0) {
                if (record_count == D_0013F350.count_value) {
                    sprintf(frame.statistics_text, D_0015F588, func_001FDD10(0x50A5));
                    func_001F6AF0(0x100, D_0015F51C + 0x56, primary_stat_colour, frame.statistics_text, -1);
                    sprintf(frame.statistics_text, D_0015F5A0, D_0013F350.count_value);
                    func_001F6AF0(0x100, D_0015F51C + 0x6A, primary_stat_colour, frame.statistics_text, -1);
                } else {
                    goto no_record_count;
                }
            } else {
no_record_count:
                sprintf(frame.statistics_text, D_0015F588, func_001FDD10(0x50A4));
                func_001F6AF0(0x100, D_0015F51C + 0x60, secondary_stat_colour, frame.statistics_text, -1);
            }
            func_001F6AF0(0x100, D_0015F51C + 0x90, frame.text_colour, func_001FDD10(0x5249), -1);
            final_text_y = D_0015F51C + 0xA8;
            final_text_colour = frame.text_colour;
            final_text = func_001FDD10(0x5248);
        }
        goto draw_final_text;
    case 2:
        message_pulse_period = func_001FA6C0(D_0015F520);
        message_pulse_frame = (s32) D_0015F438[0] % (s32) D_0015F520;
        func_001F6060(0x64, 0xA0, 0xB0, 0x150, func_001FA6E0(D_0015F524, D_0015F528, (func_001F9DE0((((f32) message_pulse_frame / message_pulse_period) * 6.28318f) - 3.14159f) * 0.5f) + 0.5f));
        final_text = D_00193300.primary_text;
        final_text_y = 0x7A;
        final_text_colour = ((s64) 0x80000000U) | 0xC0C0;
draw_final_text:
        func_001F6AF0(0x100, final_text_y, final_text_colour, final_text, -1);
        break;
    default:
        default_pulse_period = func_001FA6C0(D_0015F520);
        default_pulse_frame = (s32) D_0015F438[0] % (s32) D_0015F520;
        func_001F6060(0x50, D_0015F4EC + 0x1A, 0xB0, 0x150, func_001FA6E0(D_0015F524, D_0015F528, (func_001F9DE0((((f32) default_pulse_frame / default_pulse_period) * 6.28318f) - 3.14159f) * 0.5f) + 0.5f));
        if (D_00193300.primary_text != NULL) {
            func_001F6AF0(0x100, 0x5A, ((s64) 0x80000000U) | 0xC0C0, D_00193300.primary_text, -1);
        }
        if (D_00193300.secondary_text != NULL) {
            func_001F6AF0(0x100, D_0015F4E8, ((s64) 0x80FF0000U) | 0xA888, D_00193300.secondary_text, -1);
        }
        if (D_00193300.tertiary_text != NULL) {
            func_001F6AF0(0x100, D_0015F4EC, ((s64) 0x80FF0000U) | 0xA888, D_00193300.tertiary_text, -1);
        }
        break;
    }
    func_001F4398();
}

extern void func_001FBC50(void) __attribute__((alias("FUN_001fbc50")));
#endif /* NON_MATCHING */
