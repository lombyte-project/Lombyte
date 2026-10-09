#include "types.h"
#include "sda.h"
#include "rnc/globals.h"
#include "rnc/rendering/screen.h"
#include "rnc/ui/text/text_region.h"
#include "rnc/gameplay/hero.h"

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

extern MessageBox D_00193300;
extern s32 D_0015F438;
extern s32 D_0015F4E8 __attribute__((sda));
extern s32 D_0015F4EC __attribute__((sda));
extern s32 D_0015F4F0 __attribute__((sda));
extern s32 D_0015F4F4 __attribute__((sda));
extern s32 D_0015F4F8 __attribute__((sda));
extern s32 D_0015F4FC __attribute__((sda));
extern s32 D_0015F500 __attribute__((sda));
extern s32 D_0015F504 __attribute__((sda));
extern s32 D_0015F508 __attribute__((sda));
extern s32 D_0015F50C __attribute__((sda));
extern s32 D_0015F510 __attribute__((sda));
extern s32 D_0015F514 __attribute__((sda));
extern s32 D_0015F518 __attribute__((sda));
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
extern char D_0015F548[];
extern char D_0015F550[];
extern char D_0015F560[];
extern char D_0015F568[];
extern char D_0015F570[];
extern char D_0015F578[];
extern char D_0015F580[];
extern char D_0015F588[];
extern char D_0015F590[];
extern char D_0015F5A0[];
extern s32 D_0015F5E8;
extern u8 D_001DF050[];
extern struct TextRegion D_001E78F0;
extern struct TextRegion D_001E7908;
extern s32 D_0015EE58[] MACRO_ADDR;
extern s32 D_0015EE68[] MACRO_ADDR;

extern void memset(void *, s32, s32);
extern s32 sprintf(char *, const char *, ...);
extern void strncpy(void *, char *, s32);
extern void setup_gif_paging(s32) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern s32 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void DrawUIFrame(s32, s32, s32, s32, s32) __asm__("func_001F5F18");
extern void draw_outlined_rect(s32, s32, s32, s32, s32) __asm__("func_001F6060");
extern s32 font_print_center(s32, s32, s64, char *, s32) __asm__("func_001F6AF0");
extern void font_print_window(struct TextRegion *, s64, void *, s32, s32, u8 *) __asm__("FUN_001f7090");
extern void font_print_window_regular(struct TextRegion *, s32, char *, s32) __asm__("func_001F7580");
extern s32 scale_game_frames(s32) __asm__("FUN_001f96f8");
extern f32 fast_sin(f32) __asm__("func_001F9DE0");
extern f32 ConvertIntegerToFloat(s32) __asm__("func_001FA6C0");
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
extern s32 func_001FA6E0(s32, s32, f32);
extern char *get_help_message_text(s32) __asm__("func_001FDD10");
extern s32 find_valid_animation_frame_index_alt(s32, s32) __asm__("func_001FF960");
extern s64 get_frame_texture(s32) __asm__("func_001FFA10");
extern void draw_hud_sprite(s32, s32, s32, s32, s32, s32) __asm__("func_001FFC30");
/* Same registers as (w, h, tex, x, y, cx, cy, angle); floats are listed first so they are evaluated first. */
extern void draw_rotated_sprite(f32 x, f32 y, f32 cx, f32 cy, f32 angle, s32 w, s32 h, s64 tex) __asm__("FUN_00200600");
extern void vu1_add_g_sregister(s32, s64) __asm__("FUN_00233980");

#define SCRATCHPAD ((u8 *)0x70000000)

void draw_dialog_text(void) __asm__("FUN_001fbc50");

void draw_dialog_text(void) {
    char buf[0x200];
    struct TextRegion box;

    setup_gif_paging(0);
    switch (D_00193300.state) {
    case 5: {
        u8 *p;
        s32 color;
        s32 tint;
        s32 y;
        s32 top;
        s32 mid;
        f32 alpha;
        f32 pos;
        f32 angle;
        f32 pivot;

        alpha = 1.0f - (f32)D_00193300.timer / (f32)scale_game_frames(30);
        DrawUIFrame(0x50, 0x154, 0x60, 0x1A0, (s32)(alpha * 80.0f));
        box = D_001E78F0;
        color = func_001FA6E0(D_0015F4F0, D_0015F4F4, alpha);
        strncpy(SCRATCHPAD, get_help_message_text(0x4E2B), 0x400);
        p = SCRATCHPAD;
        while (*p >= 2) {
            p++;
        }
        if (*p == 1) {
            do {
                *p++ = 0;
            } while (*p == 1);
        }
        font_print_window(&box, color, SCRATCHPAD, -1, get_effect_texture(1), D_001DF050);
        pivot = 272.0f;
        box.flags |= 4;
        y = box.anchor_y;
        y += box.rendered_height;
        font_print_window(&box, color, p, -1, get_effect_texture(1), D_001DF050);
        box.flags ^= 4;
        box.anchor_y = 0x136 - box.rendered_height;
        font_print_window(&box, color, p, -1, get_effect_texture(1), D_001DF050);
        tint = func_001FA6E0(0x20FFFF, 0x8020FFFF,
                            1.0f - (f32)D_00193300.unk24 / (f32)scale_game_frames(30));
        font_print_center(0x100, 0x140, tint, get_help_message_text(0x524A), -1);
        mid = (y + box.anchor_y) >> 1;
        vu1_add_g_sregister(0x47, 0x3004B);
        top = mid - 0x20;
        draw_hud_sprite(find_valid_animation_frame_index_alt(0x755D, 0), 0xE0, top, 0x40, 0x40, 0x80);
        pos = (f32)(mid << 4);
        angle = (f32)(D_0015F438 % 55) * -6.2831855f / 55.0f;
        draw_rotated_sprite(4096.0f, pos, pivot, pivot, angle, 0x40, 0x40,
                            get_frame_texture(find_valid_animation_frame_index_alt(0x755D, 1)));
        break;
    }
    case 3: {
        char *text;
        s32 left;
        s32 right;
        s32 mid;
        s32 tint;
        s32 y;
        s32 h;
        f32 alpha;

        text = D_0015F548;
        left = 0;
        mid = 0;
        right = 0;
        switch (mode_freeze_state) {
        case 6:
            left = 0x524F;
            right = 0x524B;
            text = get_help_message_text(0x4FAF);
            break;
        case 12:
            if (!(mode_freeze_flags & 2)) {
                mid = 0x4FA8;
                text = get_help_message_text(0x4FA7);
                break;
            }
        case 13:
            left = 0x524F;
            right = 0x524B;
            text = get_help_message_text(0x4FAD);
            break;
        case 10:
        case 11:
            text = get_help_message_text(0x4FC1);
            break;
        case 7:
        case 8:
            text = get_help_message_text(0x4FB7);
            break;
        case 14:
        case 15:
            text = get_help_message_text(0x4FB8);
            break;
        case 17:
            mid = 0x524A;
            text = get_help_message_text(0x4FBA);
            break;
        case 18:
            mid = 0x524A;
            text = get_help_message_text(0x4FBC);
            break;
        case 2:
            mid = 0x524A;
            text = get_help_message_text(0x4FA6);
            if (D_00193300.timer != 0) {
                mid = 0;
            }
            break;
        case 19:
            if (mode_freeze_flags & 4) {
                mid = 0x4FA8;
                text = get_help_message_text(0x4FA7);
            } else if (D_0015F5E8 != 0) {
                left = 0x524E;
                right = 0x524B;
                sprintf(buf, D_0015F550, get_help_message_text(0x4FA9), 1, 1, get_help_message_text(0x4FAA));
                text = buf;
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
        case 24:
            left = 0x524E;
            right = 0x524B;
            sprintf(buf, D_0015F550, get_help_message_text(0x4FAE), 1, 1, get_help_message_text(0x4FAA));
            text = buf;
            break;
        case 23:
            left = 0x524E;
            right = 0x524B;
            text = get_help_message_text(0x4FB1);
            break;
        case 3:
        case 4:
            if (D_0015F5E8 != 0 && (mode_freeze_flags & 2)) {
                left = 0x524E;
                right = 0x524B;
                text = get_help_message_text(0x4FAB);
            } else {
                mid = 0x4FA8;
                text = get_help_message_text(0x4FAC);
            }
            break;
        case 5:
            mid = 0x4FA8;
            sprintf(buf, D_0015F550, get_help_message_text(0x4FB0), 1, 1, get_help_message_text(0x4FA7));
            text = buf;
            break;
        }
        box = (struct TextRegion){0, D_0013E500.height, 0x60, 0x1A0, 0x100, 0x68, 0, 0, 0x10, 5};
        font_print_window_regular(&box, 0, text, -1);
        h = box.rendered_height + 0x28;
        y = (D_0013E500.height - h) >> 1;
        box.bottom = y + h;
        box.anchor_y = y + 4;
        box.top = y;
        alpha = 1.0f - (f32)D_00193300.timer / (f32)scale_game_frames(30);
        DrawUIFrame(box.top, box.bottom, 0x60, 0x1A0, (s32)(alpha * 80.0f));
        box.flags ^= 4;
        font_print_window_regular(&box, func_001FA6E0(D_0015F4F0, D_0015F4F4, alpha), text, -1);
        tint = func_001FA6E0(0x20FFFF, 0x8020FFFF,
                            1.0f - (f32)D_00193300.unk24 / (f32)scale_game_frames(30));
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
    case 6: {
        s32 text_color;
        s32 button_color;
        s32 id;
        f32 alpha;

        box = D_001E7908;
        alpha = 1.0f - (f32)D_00193300.timer / (f32)scale_game_frames(30);
        DrawUIFrame(0x64, 0x12C, 0x60, 0x1A0, (s32)(alpha * 80.0f));
        text_color = func_001FA6E0(D_0015F4F0, D_0015F4F4, alpha);
        button_color = func_001FA6E0(0x20FFFF, 0x8020FFFF, alpha);
        switch (D_00193300.choice) {
        case 0:
        case 1:
            font_print_center(0xCA, 0x118, button_color, get_help_message_text(0x524E), -1);
            font_print_center(0x135, 0x118, button_color, get_help_message_text(0x524B), -1);
            id = 0x522A;
            break;
        case 2:
            font_print_center(0xCA, 0x118, button_color, get_help_message_text(0x524E), -1);
            font_print_center(0x135, 0x118, button_color, get_help_message_text(0x524B), -1);
            id = 0x522B;
            break;
        case 3:
            font_print_center(0x100, 0x118, button_color, get_help_message_text(0x524A), -1);
            id = 0x522C;
            break;
        default:
            id = 0;
            break;
        }
        if (id != 0) {
            font_print_window(&box, text_color, get_help_message_text(id), -1, get_effect_texture(1), D_001DF050);
        }
        break;
    }
    case 0: {
        char line[0x40];
        s32 pulse;
        s32 c1;
        s32 c2;
        s32 c3;
        s32 best;
        s32 record;
        s32 unit;
        s32 rem;
        s32 per;
        s32 min;
        s32 sec;
        s32 hund;
        f32 scale;
        f32 f;

        pulse = func_001FA6E0(D_0015F524, D_0015F528,
                             fast_sin((f32)(D_0015F438 % D_0015F520) / ConvertIntegerToFloat(D_0015F520) * 6.28318f - 3.14159f) *
                                     0.5f +
                                 0.5f);
        scale = (f32)D_00193300.unk20 * 0.125f;
        if (scale > 1.0f) {
            scale = 1.0f;
        } else if (scale < 0.1f) {
            scale = 0.1f;
        }
        c1 = D_0015F52C;
        c2 = D_0015F534;
        c3 = D_0015F53C;
        if (D_00193300.unk24 != 0) {
            f = (f32)D_00193300.unk24 * 0.125f;
            if (f > 1.0f) {
                f = 1.0f;
            } else if (f < 0.0f) {
                f = 0.0f;
            }
            c1 = func_001FA6E0(D_0015F52C, D_0015F530, f);
            c2 = func_001FA6E0(D_0015F534, D_0015F538, f);
            c3 = func_001FA6E0(D_0015F53C, D_0015F540, f);
        }
        if (hero.unk89A < 3) {
            draw_outlined_rect(D_0015F4FC - truncate_float_to_s32((f32)D_0015F504 * scale),
                         D_0015F4FC + truncate_float_to_s32((f32)D_0015F504 * scale),
                         D_0015F4F8 - truncate_float_to_s32((f32)D_0015F500 * scale),
                         D_0015F4F8 + truncate_float_to_s32((f32)D_0015F500 * scale), pulse);
            font_print_center(0x100, D_0015F508, c1, get_help_message_text(0x4F6E), -1);
            font_print_center(0x100, D_0015F508 + 0x18, c1, get_help_message_text(0x5249), -1);
            font_print_center(0x100, D_0015F508 + 0x30, c1, get_help_message_text(0x5248), -1);
        } else {
            draw_outlined_rect(D_0015F510 - truncate_float_to_s32((f32)D_0015F518 * scale),
                         D_0015F510 + truncate_float_to_s32((f32)D_0015F518 * scale),
                         D_0015F50C - truncate_float_to_s32((f32)D_0015F514 * scale),
                         D_0015F50C + truncate_float_to_s32((f32)D_0015F514 * scale), pulse);
            sprintf(line, D_0015F560, get_help_message_text(0x5240));
            if (hero.unk8C0 == 1) {
                sprintf(line, D_0015F568, 1);
            } else if (hero.unk8C0 == 2) {
                sprintf(line, D_0015F570, 2);
            } else if (hero.unk8C0 == 3) {
                sprintf(line, D_0015F578, 3);
            } else {
                sprintf(line, D_0015F580, hero.unk8C0);
            }
            line[3] = ' ';
            if (hero.unk8C0 == 1) {
                font_print_center(0x100, D_0015F51C, c2, line, -1);
            } else {
                font_print_center(0x100, D_0015F51C, c3, line, -1);
            }
            best = D_0015EE58[(current_level_index ^ 0x10) ? 0 : 1];
            if (best == hero.unk894) {
                unit = pal_mode ? 3000 : 3600;
                per = unit / 60;
                min = best / unit;
                rem = best - min * unit;
                sec = rem / per;
                hund = (rem - sec * per) * 100 / per;
                sprintf(line, D_0015F588, get_help_message_text(0x50A3));
                font_print_center(0x100, D_0015F51C + 0x26, c2, line, -1);
                sprintf(line, D_0015F590, min, sec, hund);
                font_print_center(0x100, D_0015F51C + 0x3A, c2, line, -1);
            } else {
                sprintf(line, D_0015F588, get_help_message_text(0x50A2));
                font_print_center(0x100, D_0015F51C + 0x30, c3, line, -1);
            }
            record = D_0015EE68[(current_level_index ^ 0x10) ? 0 : 1];
            if (record != 0 && record == hero.unk8A8) {
                sprintf(line, D_0015F588, get_help_message_text(0x50A5));
                font_print_center(0x100, D_0015F51C + 0x56, c2, line, -1);
                sprintf(line, D_0015F5A0, hero.unk8A8);
                font_print_center(0x100, D_0015F51C + 0x6A, c2, line, -1);
            } else {
                sprintf(line, D_0015F588, get_help_message_text(0x50A4));
                font_print_center(0x100, D_0015F51C + 0x60, c3, line, -1);
            }
            font_print_center(0x100, D_0015F51C + 0x90, c1, get_help_message_text(0x5249), -1);
            font_print_center(0x100, D_0015F51C + 0xA8, c1, get_help_message_text(0x5248), -1);
        }
        break;
    }
    case 2:
        draw_outlined_rect(0x64, 0xA0, 0xB0, 0x150,
                     func_001FA6E0(D_0015F524, D_0015F528,
                                  fast_sin((f32)(D_0015F438 % D_0015F520) / ConvertIntegerToFloat(D_0015F520) * 6.28318f -
                                               3.14159f) *
                                          0.5f +
                                      0.5f));
        font_print_center(0x100, 0x7A, 0x8000C0C0, D_00193300.title, -1);
        break;
    default: {
        MessageBox *mb = &D_00193300;
        s32 pulse;

        pulse = func_001FA6E0(D_0015F524, D_0015F528,
                                  fast_sin((f32)(D_0015F438 % D_0015F520) / ConvertIntegerToFloat(D_0015F520) * 6.28318f -
                                               3.14159f) *
                                          0.5f +
                                      0.5f);
        draw_outlined_rect(0x50, D_0015F4EC + 0x1A, 0xB0, 0x150, pulse);
        if (mb->title != NULL) {
            font_print_center(0x100, 0x5A, 0x8000C0C0, mb->title, -1);
        }
        if (mb->line1 != NULL) {
            font_print_center(0x100, D_0015F4E8, 0x80FFA888, mb->line1, -1);
        }
        if (mb->line2 != NULL) {
            font_print_center(0x100, D_0015F4EC, 0x80FFA888, mb->line2, -1);
        }
        break;
    }
    }
    do_gif_paging();
}

extern __typeof__(draw_dialog_text) func_001FBC50 __attribute__((alias("FUN_001fbc50")));
