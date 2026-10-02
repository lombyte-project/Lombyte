#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/draw_dialog_text/FUN_001fbc50.s", FUN_001fbc50);
#else
#include "types.h"
#include "sda.h"

typedef struct {
    short s[12];
} TextBox;

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
extern DialogState D_00193300;
extern u8 D_001DF050[];
extern TextBox D_001E78F0;
extern TextBox D_001E7908;

extern void func_001F4280(int);
extern void func_001F4398(void);
extern int func_001F44B8(int);
extern void func_001F5F18(int, int, int, int, int);
extern void func_001F6060(int, int, int, int, int);
extern void func_001F6AF0(int, int, long, char *, int);
extern void func_001F7090(TextBox *, long, char *, int, int, void *);
extern void func_001F7580(TextBox *, long, char *, int);
extern int func_001F96F8(int);
extern float func_001F9DE0(float);
extern float func_001FA6C0(int);
extern int func_001FA6D0(float);
extern int func_001FA6E0(int, int, float);
extern char *func_001FDD10(int);
extern int func_001FF960(int, int);
extern int func_001FFA10(int);
extern void func_001FFC30(int, int, int, int, int, int);
extern void func_00200600(int, int, int, float, float, float, float, float);
extern void func_00233980(int, long);
extern void *memset(void *, int, unsigned int);
extern int sprintf(char *, const char *, ...);
extern char *strncpy(char *, const char *, int);

void FUN_001fbc50(void) {
    char buf[0x200];
    TextBox box;
    float t;
    float scale;
    float blink;
    float size;
    float x;
    float ang;
    int color;
    int color2;
    int txtcol;
    int col_hi;
    int col_lo;
    int pulse;
    char *text;
    u8 *p;
    char *arg;
    const char *fmt;
    int id;
    int yes;
    int no;
    int ok;
    int y;
    int mid;
    int ph;
    int frames;
    int total;
    int per_sec;
    int min;
    int sec;
    int hund;
    float t5;
    float t6;
    int y5;
    int color5;
    int color6;

    func_001F4280(0);
    switch (D_00193300.mode) {
    case 5:
        t5 = 1.0f - (float)D_00193300.fade_in / (float)func_001F96F8(30);
        func_001F5F18(0x50, 0x154, 0x60, 0x1A0, (int)(t5 * 80.0f));
        box = D_001E78F0;
        color5 = func_001FA6E0(D_0015F4F0, D_0015F4F4, t5);
        strncpy((char *)0x70000000, func_001FDD10(0x4E2B), 0x400);
        p = (u8 *)0x70000000;
        while (*p >= 2) {
            p++;
        }
        while (*p == 1) {
            *p = 0;
            p++;
        }
        func_001F7090(&box, color5, (char *)0x70000000, -1, func_001F44B8(1), D_001DF050);
        size = 272.0f;
        box.s[9] |= 4;
        mid = box.s[5];
        mid += box.s[7];
        func_001F7090(&box, color5, (char *)p, -1, func_001F44B8(1), D_001DF050);
        box.s[9] ^= 4;
        box.s[5] = 0x136 - box.s[7];
        func_001F7090(&box, color5, (char *)p, -1, func_001F44B8(1), D_001DF050);
        color5 = func_001FA6E0(0x20FFFF, 0x8020FFFF,
                              1.0f - (float)D_00193300.fade_out / (float)func_001F96F8(30));
        func_001F6AF0(0x100, 0x140, color5, func_001FDD10(0x524A), -1);
        mid = (mid + box.s[5]) >> 1;
        func_00233980(0x47, 0x3004B);
        y5 = mid - 0x20;
        func_001FFC30(func_001FF960(0x755D, 0), 0xE0, y5, 0x40, 0x40, 0x80);
        ph = D_0015F438 % 55;
        x = (float)(mid * 16);
        ang = (float)ph * -6.2831855f / 55.0f;
        func_00200600(0x40, 0x40, func_001FFA10(func_001FF960(0x755D, 1)), 4096.0f, x, size, size, ang);
        break;

    case 3:
        text = D_0015F548;
        yes = 0;
        ok = 0;
        no = 0;
        switch (D_0015EEB0) {
        case 6:
            id = 0x4FAF;
            yes = 0x524F;
            goto two;
        case 12:
            id = 0x4FA7;
            if (!(D_0015EEB4 & 2)) {
                goto back;
            }
        case 13:
            id = 0x4FAD;
            yes = 0x524F;
            goto two;
        case 10:
        case 11:
            id = 0x4FC1;
            goto one;
        case 7:
        case 8:
            id = 0x4FB7;
            goto one;
        case 14:
        case 15:
            id = 0x4FB8;
            goto one;
        case 17:
            id = 0x4FBA;
            ok = 0x524A;
            goto one;
        case 18:
            id = 0x4FBC;
            ok = 0x524A;
            goto one;
        case 2:
            text = func_001FDD10(0x4FA6);
            ok = 0x524A;
            if (D_00193300.fade_in != 0) {
                ok = 0;
            }
            break;
        case 19:
            id = 0x4FA7;
            if (D_0015EEB4 & 4) {
                goto back;
            }
            if (D_0015F5E8 != 0) {
                fmt = D_0015F550;
                yes = 0x524E;
                no = 0x524B;
                arg = func_001FDD10(0x4FA9);
                id = 0x4FAA;
                goto print;
            }
            id = 0x4FA9;
            goto back;
        case 21:
            id = 0x4FBD;
            ok = 0x524A;
            goto one;
        case 20:
            id = 0x4FBB;
            ok = 0x524A;
            goto one;
        case 24:
            no = 0x524B;
            yes = 0x524E;
            arg = func_001FDD10(0x4FAE);
            fmt = D_0015F550;
            id = 0x4FAA;
            goto print;
        case 23:
            id = 0x4FB1;
            goto yesno;
        case 3:
        case 4:
            if (D_0015F5E8 != 0 && (D_0015EEB4 & 2)) {
                id = 0x4FAB;
            yesno:
                yes = 0x524E;
            two:
                no = 0x524B;
                text = func_001FDD10(id);
                break;
            }
            id = 0x4FAC;
        back:
            ok = 0x4FA8;
        one:
            text = func_001FDD10(id);
            break;
        case 5:
            fmt = D_0015F550;
            ok = 0x4FA8;
            arg = func_001FDD10(0x4FB0);
            id = 0x4FA7;
        print:
            sprintf(buf, fmt, arg, 1, 1, func_001FDD10(id));
            text = buf;
            break;
        }
        box = (TextBox){ { 0, D_0013E500[1], 0x60, 0x1A0, 0x100, 0x68, 0, 0, 0x10, 5 } };
        func_001F7580(&box, 0, text, -1);
        y = box.s[7] + 0x28;
        mid = (D_0013E500[1] - y) >> 1;
        box.s[5] = mid + 4;
        box.s[1] = mid + y;
        box.s[0] = mid;
        t6 = 1.0f - (float)D_00193300.fade_in / (float)func_001F96F8(30);
        func_001F5F18(box.s[0], box.s[1], 0x60, 0x1A0, (int)(t6 * 80.0f));
        box.s[9] ^= 4;
        func_001F7580(&box, func_001FA6E0(D_0015F4F0, D_0015F4F4, t6), text, -1);
        color6 = func_001FA6E0(0x20FFFF, 0x8020FFFF,
                              1.0f - (float)D_00193300.fade_out / (float)func_001F96F8(30));
        if (yes != 0) {
            func_001F6AF0(0xCA, box.s[1] - 0x14, color6, func_001FDD10(yes), -1);
        }
        if (no != 0) {
            func_001F6AF0(0x135, box.s[1] - 0x14, color6, func_001FDD10(no), -1);
        }
        if (ok != 0) {
            func_001F6AF0(0x100, box.s[1] - 0x14, color6, func_001FDD10(ok), -1);
        }
        break;

    case 6:
        box = D_001E7908;
        t6 = 1.0f - (float)D_00193300.fade_in / (float)func_001F96F8(30);
        func_001F5F18(0x64, 0x12C, 0x60, 0x1A0, (int)(t6 * 80.0f));
        color6 = func_001FA6E0(D_0015F4F0, D_0015F4F4, t6);
        color2 = func_001FA6E0(0x20FFFF, 0x8020FFFF, t6);
        switch (D_00193300.choice) {
        case 0:
        case 1:
            func_001F6AF0(0xCA, 0x118, color2, func_001FDD10(0x524E), -1);
            func_001F6AF0(0x135, 0x118, color2, func_001FDD10(0x524B), -1);
            id = 0x522A;
            break;
        case 2:
            func_001F6AF0(0xCA, 0x118, color2, func_001FDD10(0x524E), -1);
            func_001F6AF0(0x135, 0x118, color2, func_001FDD10(0x524B), -1);
            id = 0x522B;
            break;
        case 3:
            func_001F6AF0(0x100, 0x118, color2, func_001FDD10(0x524A), -1);
            id = 0x522C;
            break;
        default:
            id = 0;
            break;
        }
        if (id != 0) {
            func_001F7090(&box, color6, func_001FDD10(id), -1, func_001F44B8(1), D_001DF050);
        }
        break;

    case 0:
        pulse = func_001FA6E0(D_0015F524, D_0015F528,
                              func_001F9DE0((float)(D_0015F438 % D_0015F520) / func_001FA6C0(D_0015F520) * 6.28318f - 3.14159f)
                                  * 0.5f + 0.5f);
        scale = (float)D_00193300.blink * 0.125f;
        if (scale > 1.0f) {
            scale = 1.0f;
        } else if (scale < 0.1f) {
            scale = 0.1f;
        }
        txtcol = D_0015F52C;
        col_hi = D_0015F534;
        col_lo = D_0015F53C;
        if (D_00193300.fade_out != 0) {
            blink = (float)D_00193300.fade_out * 0.125f;
            if (blink > 1.0f) {
                blink = 1.0f;
            } else if (blink < 0.0f) {
                blink = 0.0f;
            }
            txtcol = func_001FA6E0(D_0015F52C, D_0015F530, blink);
            col_hi = func_001FA6E0(D_0015F534, D_0015F538, blink);
            col_lo = func_001FA6E0(D_0015F53C, D_0015F540, blink);
        }
        if (D_0013F350.x89A < 3) {
            func_001F6060(D_0015F4FC - func_001FA6D0((float)D_0015F504 * scale),
                          D_0015F4FC + func_001FA6D0((float)D_0015F504 * scale),
                          D_0015F4F8 - func_001FA6D0((float)D_0015F500 * scale),
                          D_0015F4F8 + func_001FA6D0((float)D_0015F500 * scale), pulse);
            func_001F6AF0(0x100, D_0015F508, txtcol, func_001FDD10(0x4F6E), -1);
            func_001F6AF0(0x100, D_0015F508 + 0x18, txtcol, func_001FDD10(0x5249), -1);
            func_001F6AF0(0x100, D_0015F508 + 0x30, txtcol, func_001FDD10(0x5248), -1);
        } else {
            char line[0x40];

            func_001F6060(D_0015F510 - func_001FA6D0((float)D_0015F518 * scale),
                          D_0015F510 + func_001FA6D0((float)D_0015F518 * scale),
                          D_0015F50C - func_001FA6D0((float)D_0015F514 * scale),
                          D_0015F50C + func_001FA6D0((float)D_0015F514 * scale), pulse);
            sprintf(line, D_0015F560, func_001FDD10(0x5240));
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
                func_001F6AF0(0x100, D_0015F51C, col_hi, line, -1);
            } else {
                func_001F6AF0(0x100, D_0015F51C, col_lo, line, -1);
            }
            total = D_0015EE58[(D_0015ED84 ^ 0x10) ? 0 : 1];
            if (total == D_0013F350.best_time) {
                per_sec = D_0015ED80 ? 3000 : 3600;
                min = total / per_sec;
                frames = per_sec / 60;
                per_sec = total - min * per_sec;
                sec = per_sec / frames;
                per_sec -= sec * frames;
                hund = per_sec * 100 / frames;
                sprintf(line, D_0015F588, func_001FDD10(0x50A3));
                func_001F6AF0(0x100, D_0015F51C + 0x26, col_hi, line, -1);
                sprintf(line, D_0015F590, min, sec, hund);
                func_001F6AF0(0x100, D_0015F51C + 0x3A, col_hi, line, -1);
            } else {
                sprintf(line, D_0015F588, func_001FDD10(0x50A2));
                func_001F6AF0(0x100, D_0015F51C + 0x30, col_lo, line, -1);
            }
            total = D_0015EE68[(D_0015ED84 ^ 0x10) ? 0 : 1];
            if (total != 0 && total == D_0013F350.best_score) {
                sprintf(line, D_0015F588, func_001FDD10(0x50A5));
                func_001F6AF0(0x100, D_0015F51C + 0x56, col_hi, line, -1);
                sprintf(line, D_0015F5A0, D_0013F350.best_score);
                func_001F6AF0(0x100, D_0015F51C + 0x6A, col_hi, line, -1);
            } else {
                sprintf(line, D_0015F588, func_001FDD10(0x50A4));
                func_001F6AF0(0x100, D_0015F51C + 0x60, col_lo, line, -1);
            }
            func_001F6AF0(0x100, D_0015F51C + 0x90, txtcol, func_001FDD10(0x5249), -1);
            func_001F6AF0(0x100, D_0015F51C + 0xA8, txtcol, func_001FDD10(0x5248), -1);
        }
        break;

    case 2:
        func_001F6060(0x64, 0xA0, 0xB0, 0x150,
                      func_001FA6E0(D_0015F524, D_0015F528,
                                    func_001F9DE0((float)(D_0015F438 % D_0015F520) / func_001FA6C0(D_0015F520) * 6.28318f - 3.14159f)
                                        * 0.5f + 0.5f));
        func_001F6AF0(0x100, 0x7A, 0x8000C0C0, D_00193300.line1, -1);
        break;

    default:
        pulse = func_001FA6E0(D_0015F524, D_0015F528,
                              func_001F9DE0((float)(D_0015F438 % D_0015F520) / func_001FA6C0(D_0015F520) * 6.28318f - 3.14159f)
                                  * 0.5f + 0.5f);
        func_001F6060(0x50, D_0015F4EC + 0x1A, 0xB0, 0x150, pulse);
        if (D_00193300.line1 != 0) {
            func_001F6AF0(0x100, 0x5A, 0x8000C0C0, D_00193300.line1, -1);
        }
        if (D_00193300.line2 != 0) {
            func_001F6AF0(0x100, D_0015F4E8, 0x80FFA888, D_00193300.line2, -1);
        }
        if (D_00193300.line3 != 0) {
            func_001F6AF0(0x100, D_0015F4EC, 0x80FFA888, D_00193300.line3, -1);
        }
        break;
    }
    func_001F4398();
}
#endif /* NON_MATCHING */
