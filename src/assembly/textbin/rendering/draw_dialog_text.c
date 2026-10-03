#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/draw_dialog_text/FUN_001fbc50.s", FUN_001fbc50);
#else
#include "types.h"
#include "sda.h"

typedef struct {
    short s[12];
} FontWindow;

typedef struct {
    s32 mode;           /* 0x00 */
    s32 fade_in;        /* 0x04 */
    char *line1;        /* 0x08 */
    char *line2;        /* 0x0C */
    char *line3;        /* 0x10 */
    s32 pad14[2];
    s32 choice;         /* 0x1C */
    s32 blink;          /* 0x20 */
    s32 fade_out;       /* 0x24 */
} DialogState;

extern s32 D_0013E500[];
typedef struct {
    u8 pad0[0x894];
    s32 best_time;      /* 0x894 */
    u8 pad898[2];
    s16 x89A;           /* 0x89A */
    u8 pad89C[0xC];
    s32 best_score;     /* 0x8A8 */
    u8 pad8AC[0x14];
    s32 place;          /* 0x8C0 */
} GameState;
extern GameState D_0013F350;
extern s32 D_0015ED80;
extern s32 D_0015ED84;
extern s32 D_0015EE58[];
extern s32 D_0015EE68[];
extern s32 D_0015EEB0;
extern s32 D_0015EEB4;
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
extern DialogState dialog_state __asm__("D_00193300");
extern u8 D_001DF050[];
extern FontWindow D_001E78F0;
extern FontWindow D_001E7908;

extern void setup_gif_paging(int) __asm__("func_001F4280");
extern void do_gif_paging(void) __asm__("func_001F4398");
extern u64 get_effect_texture(int) __asm__("func_001F44B8");
extern void func_001F5F18(int, int, int, int, int);
extern void func_001F6060(int, int, int, int, int);
extern void font_print_center(int, int, long, char *, int) __asm__("func_001F6AF0");
extern void font_print_window(FontWindow *, u64, char *, int, s64, void *) __asm__("func_001F7090");
extern void font_print_window_regular(FontWindow *, long, char *, int) __asm__("func_001F7580");
extern int func_001F96F8(int);
extern float fast_sin(float) __asm__("func_001F9DE0");
extern float func_001FA6C0(int);
extern int func_001FA6D0(float);
extern int func_001FA6E0(int, int, float);
extern char *get_help_message_text(int) __asm__("func_001FDD10");
extern int func_001FF960(int, int);
extern u64 get_frame_texture(int) __asm__("func_001FFA10");
extern void func_001FFC30(int, int, int, int, int, int);
extern void func_00200600(int, int, u64, float, float, float, float, float);
extern void vu1_add_g_sregister(int, long) __asm__("func_00233980");
extern void *memset(void *, int, unsigned int);
extern int sprintf(char *, const char *, ...);
extern char *strncpy(char *, const char *, int);

void draw_dialog_text(void) __asm__("FUN_001fbc50");

void draw_dialog_text(void) {
    char text_buffer[0x200];
    FontWindow text_window;
    float t;
    float scale;
    float blink;
    float size;
    float x;
    float angle;
    int color;
    int color2;
    int text_color;
    int highlight_color;
    int normal_color;
    int background_color;
    char *text;
    u8 *text_cursor;
    char *format_argument;
    const char *format;
    int text_id;
    int yes_text_id;
    int no_text_id;
    int confirm_text_id;
    int y;
    int middle_y;
    int animation_frame;
    int frames_per_second;
    int total_frames;
    int frames_per_minute;
    int minutes;
    int seconds;
    int hundredths;
    float mode5_fade;
    float dialog_fade;
    int icon_y;
    int mode5_color;
    int dialog_color;

    setup_gif_paging(0);
    switch (dialog_state.mode) {
    case 5:
        mode5_fade = 1.0f - (float)dialog_state.fade_in / (float)func_001F96F8(30);
        func_001F5F18(0x50, 0x154, 0x60, 0x1A0, (int)(mode5_fade * 80.0f));
        text_window = D_001E78F0;
        mode5_color = func_001FA6E0(D_0015F4F0, D_0015F4F4, mode5_fade);
        strncpy((char *)0x70000000, get_help_message_text(0x4E2B), 0x400);
        text_cursor = (u8 *)0x70000000;
        while (*text_cursor >= 2) {
            text_cursor++;
        }
        while (*text_cursor == 1) {
            *text_cursor = 0;
            text_cursor++;
        }
        font_print_window(&text_window, mode5_color, (char *)0x70000000, -1, get_effect_texture(1), D_001DF050);
        size = 272.0f;
        text_window.s[9] |= 4;
        middle_y = text_window.s[5];
        middle_y += text_window.s[7];
        font_print_window(&text_window, mode5_color, (char *)text_cursor, -1, get_effect_texture(1), D_001DF050);
        text_window.s[9] ^= 4;
        text_window.s[5] = 0x136 - text_window.s[7];
        font_print_window(&text_window, mode5_color, (char *)text_cursor, -1, get_effect_texture(1), D_001DF050);
        mode5_color = func_001FA6E0(0x20FFFF, 0x8020FFFF,
                              1.0f - (float)dialog_state.fade_out / (float)func_001F96F8(30));
        font_print_center(0x100, 0x140, mode5_color, get_help_message_text(0x524A), -1);
        middle_y = (middle_y + text_window.s[5]) >> 1;
        vu1_add_g_sregister(0x47, 0x3004B);
        icon_y = middle_y - 0x20;
        func_001FFC30(func_001FF960(0x755D, 0), 0xE0, icon_y, 0x40, 0x40, 0x80);
        animation_frame = D_0015F438 % 55;
        x = (float)(middle_y * 16);
        angle = (float)animation_frame * -6.2831855f / 55.0f;
        func_00200600(0x40, 0x40, get_frame_texture(func_001FF960(0x755D, 1)), 4096.0f, x, size, size, angle);
        break;

    case 3:
        text = D_0015F548;
        yes_text_id = 0;
        confirm_text_id = 0;
        no_text_id = 0;
        switch (D_0015EEB0) {
        case 6:
            text_id = 0x4FAF;
            yes_text_id = 0x524F;
            goto two;
        case 12:
            text_id = 0x4FA7;
            if (!(D_0015EEB4 & 2)) {
                goto back;
            }
        case 13:
            text_id = 0x4FAD;
            yes_text_id = 0x524F;
            goto two;
        case 10:
        case 11:
            text_id = 0x4FC1;
            goto one;
        case 7:
        case 8:
            text_id = 0x4FB7;
            goto one;
        case 14:
        case 15:
            text_id = 0x4FB8;
            goto one;
        case 17:
            text_id = 0x4FBA;
            confirm_text_id = 0x524A;
            goto one;
        case 18:
            text_id = 0x4FBC;
            confirm_text_id = 0x524A;
            goto one;
        case 2:
            text = get_help_message_text(0x4FA6);
            confirm_text_id = 0x524A;
            if (dialog_state.fade_in != 0) {
                confirm_text_id = 0;
            }
            break;
        case 19:
            text_id = 0x4FA7;
            if (D_0015EEB4 & 4) {
                goto back;
            }
            if (D_0015F5E8 != 0) {
                format = D_0015F550;
                yes_text_id = 0x524E;
                no_text_id = 0x524B;
                format_argument = get_help_message_text(0x4FA9);
                text_id = 0x4FAA;
                goto print;
            }
            text_id = 0x4FA9;
            goto back;
        case 21:
            text_id = 0x4FBD;
            confirm_text_id = 0x524A;
            goto one;
        case 20:
            text_id = 0x4FBB;
            confirm_text_id = 0x524A;
            goto one;
        case 24:
            no_text_id = 0x524B;
            yes_text_id = 0x524E;
            format_argument = get_help_message_text(0x4FAE);
            format = D_0015F550;
            text_id = 0x4FAA;
            goto print;
        case 23:
            text_id = 0x4FB1;
            goto yesno;
        case 3:
        case 4:
            if (D_0015F5E8 != 0 && (D_0015EEB4 & 2)) {
                text_id = 0x4FAB;
            yesno:
                yes_text_id = 0x524E;
            two:
                no_text_id = 0x524B;
                text = get_help_message_text(text_id);
                break;
            }
            text_id = 0x4FAC;
        back:
            confirm_text_id = 0x4FA8;
        one:
            text = get_help_message_text(text_id);
            break;
        case 5:
            format = D_0015F550;
            confirm_text_id = 0x4FA8;
            format_argument = get_help_message_text(0x4FB0);
            text_id = 0x4FA7;
        print:
            sprintf(text_buffer, format, format_argument, 1, 1, get_help_message_text(text_id));
            text = text_buffer;
            break;
        }
        text_window = (FontWindow){ { 0, D_0013E500[1], 0x60, 0x1A0, 0x100, 0x68, 0, 0, 0x10, 5 } };
        font_print_window_regular(&text_window, 0, text, -1);
        y = text_window.s[7] + 0x28;
        middle_y = (D_0013E500[1] - y) >> 1;
        text_window.s[5] = middle_y + 4;
        text_window.s[1] = middle_y + y;
        text_window.s[0] = middle_y;
        dialog_fade = 1.0f - (float)dialog_state.fade_in / (float)func_001F96F8(30);
        func_001F5F18(text_window.s[0], text_window.s[1], 0x60, 0x1A0, (int)(dialog_fade * 80.0f));
        text_window.s[9] ^= 4;
        font_print_window_regular(&text_window, func_001FA6E0(D_0015F4F0, D_0015F4F4, dialog_fade), text, -1);
        dialog_color = func_001FA6E0(0x20FFFF, 0x8020FFFF,
                              1.0f - (float)dialog_state.fade_out / (float)func_001F96F8(30));
        if (yes_text_id != 0) {
            font_print_center(0xCA, text_window.s[1] - 0x14, dialog_color, get_help_message_text(yes_text_id), -1);
        }
        if (no_text_id != 0) {
            font_print_center(0x135, text_window.s[1] - 0x14, dialog_color, get_help_message_text(no_text_id), -1);
        }
        if (confirm_text_id != 0) {
            font_print_center(0x100, text_window.s[1] - 0x14, dialog_color, get_help_message_text(confirm_text_id), -1);
        }
        break;

    case 6:
        text_window = D_001E7908;
        dialog_fade = 1.0f - (float)dialog_state.fade_in / (float)func_001F96F8(30);
        func_001F5F18(0x64, 0x12C, 0x60, 0x1A0, (int)(dialog_fade * 80.0f));
        dialog_color = func_001FA6E0(D_0015F4F0, D_0015F4F4, dialog_fade);
        color2 = func_001FA6E0(0x20FFFF, 0x8020FFFF, dialog_fade);
        switch (dialog_state.choice) {
        case 0:
        case 1:
            font_print_center(0xCA, 0x118, color2, get_help_message_text(0x524E), -1);
            font_print_center(0x135, 0x118, color2, get_help_message_text(0x524B), -1);
            text_id = 0x522A;
            break;
        case 2:
            font_print_center(0xCA, 0x118, color2, get_help_message_text(0x524E), -1);
            font_print_center(0x135, 0x118, color2, get_help_message_text(0x524B), -1);
            text_id = 0x522B;
            break;
        case 3:
            font_print_center(0x100, 0x118, color2, get_help_message_text(0x524A), -1);
            text_id = 0x522C;
            break;
        default:
            text_id = 0;
            break;
        }
        if (text_id != 0) {
            font_print_window(&text_window, dialog_color, get_help_message_text(text_id), -1, get_effect_texture(1), D_001DF050);
        }
        break;

    case 0:
        background_color = func_001FA6E0(D_0015F524, D_0015F528,
                              fast_sin((float)(D_0015F438 % D_0015F520) / func_001FA6C0(D_0015F520) * 6.28318f - 3.14159f)
                                  * 0.5f + 0.5f);
        scale = (float)dialog_state.blink * 0.125f;
        if (scale > 1.0f) {
            scale = 1.0f;
        } else if (scale < 0.1f) {
            scale = 0.1f;
        }
        text_color = D_0015F52C;
        highlight_color = D_0015F534;
        normal_color = D_0015F53C;
        if (dialog_state.fade_out != 0) {
            blink = (float)dialog_state.fade_out * 0.125f;
            if (blink > 1.0f) {
                blink = 1.0f;
            } else if (blink < 0.0f) {
                blink = 0.0f;
            }
            text_color = func_001FA6E0(D_0015F52C, D_0015F530, blink);
            highlight_color = func_001FA6E0(D_0015F534, D_0015F538, blink);
            normal_color = func_001FA6E0(D_0015F53C, D_0015F540, blink);
        }
        if (D_0013F350.x89A < 3) {
            func_001F6060(D_0015F4FC - func_001FA6D0((float)D_0015F504 * scale),
                          D_0015F4FC + func_001FA6D0((float)D_0015F504 * scale),
                          D_0015F4F8 - func_001FA6D0((float)D_0015F500 * scale),
                          D_0015F4F8 + func_001FA6D0((float)D_0015F500 * scale), background_color);
            font_print_center(0x100, D_0015F508, text_color, get_help_message_text(0x4F6E), -1);
            font_print_center(0x100, D_0015F508 + 0x18, text_color, get_help_message_text(0x5249), -1);
            font_print_center(0x100, D_0015F508 + 0x30, text_color, get_help_message_text(0x5248), -1);
        } else {
            char line[0x40];

            func_001F6060(D_0015F510 - func_001FA6D0((float)D_0015F518 * scale),
                          D_0015F510 + func_001FA6D0((float)D_0015F518 * scale),
                          D_0015F50C - func_001FA6D0((float)D_0015F514 * scale),
                          D_0015F50C + func_001FA6D0((float)D_0015F514 * scale), background_color);
            sprintf(line, D_0015F560, get_help_message_text(0x5240));
            if (D_0013F350.place == 1) {
                sprintf(line, D_0015F568, 1);
            } else if (D_0013F350.place == 2) {
                sprintf(line, D_0015F570, 2);
            } else if (D_0013F350.place == 3) {
                sprintf(line, D_0015F578, 3);
            } else {
                sprintf(line, D_0015F580, D_0013F350.place);
            }
            line[3] = ' ';
            if (D_0013F350.place == 1) {
                font_print_center(0x100, D_0015F51C, highlight_color, line, -1);
            } else {
                font_print_center(0x100, D_0015F51C, normal_color, line, -1);
            }
            total_frames = D_0015EE58[(D_0015ED84 ^ 0x10) ? 0 : 1];
            if (total_frames == D_0013F350.best_time) {
                frames_per_minute = D_0015ED80 ? 3000 : 3600;
                minutes = total_frames / frames_per_minute;
                frames_per_second = frames_per_minute / 60;
                frames_per_minute = total_frames - minutes * frames_per_minute;
                seconds = frames_per_minute / frames_per_second;
                frames_per_minute -= seconds * frames_per_second;
                hundredths = frames_per_minute * 100 / frames_per_second;
                sprintf(line, D_0015F588, get_help_message_text(0x50A3));
                font_print_center(0x100, D_0015F51C + 0x26, highlight_color, line, -1);
                sprintf(line, D_0015F590, minutes, seconds, hundredths);
                font_print_center(0x100, D_0015F51C + 0x3A, highlight_color, line, -1);
            } else {
                sprintf(line, D_0015F588, get_help_message_text(0x50A2));
                font_print_center(0x100, D_0015F51C + 0x30, normal_color, line, -1);
            }
            total_frames = D_0015EE68[(D_0015ED84 ^ 0x10) ? 0 : 1];
            if (total_frames != 0 && total_frames == D_0013F350.best_score) {
                sprintf(line, D_0015F588, get_help_message_text(0x50A5));
                font_print_center(0x100, D_0015F51C + 0x56, highlight_color, line, -1);
                sprintf(line, D_0015F5A0, D_0013F350.best_score);
                font_print_center(0x100, D_0015F51C + 0x6A, highlight_color, line, -1);
            } else {
                sprintf(line, D_0015F588, get_help_message_text(0x50A4));
                font_print_center(0x100, D_0015F51C + 0x60, normal_color, line, -1);
            }
            font_print_center(0x100, D_0015F51C + 0x90, text_color, get_help_message_text(0x5249), -1);
            font_print_center(0x100, D_0015F51C + 0xA8, text_color, get_help_message_text(0x5248), -1);
        }
        break;

    case 2:
        func_001F6060(0x64, 0xA0, 0xB0, 0x150,
                      func_001FA6E0(D_0015F524, D_0015F528,
                                    fast_sin((float)(D_0015F438 % D_0015F520) / func_001FA6C0(D_0015F520) * 6.28318f - 3.14159f)
                                        * 0.5f + 0.5f));
        font_print_center(0x100, 0x7A, 0x8000C0C0, dialog_state.line1, -1);
        break;

    default:
        background_color = func_001FA6E0(D_0015F524, D_0015F528,
                              fast_sin((float)(D_0015F438 % D_0015F520) / func_001FA6C0(D_0015F520) * 6.28318f - 3.14159f)
                                  * 0.5f + 0.5f);
        func_001F6060(0x50, D_0015F4EC + 0x1A, 0xB0, 0x150, background_color);
        if (dialog_state.line1 != 0) {
            font_print_center(0x100, 0x5A, 0x8000C0C0, dialog_state.line1, -1);
        }
        if (dialog_state.line2 != 0) {
            font_print_center(0x100, D_0015F4E8, 0x80FFA888, dialog_state.line2, -1);
        }
        if (dialog_state.line3 != 0) {
            font_print_center(0x100, D_0015F4EC, 0x80FFA888, dialog_state.line3, -1);
        }
        break;
    }
    do_gif_paging();
}
extern __typeof__(draw_dialog_text) func_001FBC50 __attribute__((alias("FUN_001fbc50")));
#endif /* NON_MATCHING */
