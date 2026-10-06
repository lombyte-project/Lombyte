/* Ported from rac1-decomp (src/game/draw.c, func_001F4F90). */
#include "sda.h"
typedef struct {
    short start;   /* 0x0 */
    short end;     /* 0x2 */
    short text[6]; /* 0x4: string offsets, one per language */
} Subtitle;
typedef struct {
    char pad00[0x34];
    int time; /* 0x34 */
    char pad38[0x14];
    char *subs; /* 0x4C */
} SubState;
extern SubState D_0018CB20;
extern int D_0015ED88 MACRO_ADDR;
extern int D_0013E500[];
extern void font_set_window(void *arg0, int a1, int a2, int a3, int a4, int a5, int a6, int a7,
                                int a8);
extern void font_print_window_regular(void *, long, char *, int) __asm__("func_001F7580");
extern void draw_ui_frame(int, int, int, int, int) __asm__("func_001F5F18");
/* Draws the subtitle showing at the current time (D_0018CB20+0x34): the
   list at +0x4C holds 16-byte entries (start, end, and one string offset
   into the list per language; a negative start ends it). The language
   D_0015ED88 picks the string (2..5 map to 1..4, anything else to 0). The
   text is measured in a FontSetWindow buffer (font_set_window/7560), its
   box is kept 0x14 above the bottom of the screen (D_0013E500[1]), the
   frame is drawn (func_001F5F18) and the text printed with the measure
   flag (4) cleared. The list is tested and then read again for the
   loop, which gives retail's copy of it; the clamp test is written
   bottom-first, which gives retail's registers. */
void draw_subtitles(void) __asm__("FUN_001f4be0");

void draw_subtitles(void) {
    Subtitle *p;
    int lang;
    int idx;

    if (D_0018CB20.subs == 0) {
        return;
    }
    lang = D_0015ED88;
    idx = (lang >= 2 && lang <= 5) ? lang - 1 : 0;
    for (p = (Subtitle *)D_0018CB20.subs; p->start >= 0; p++) {
        if (D_0018CB20.time < p->start) {
            continue;
        }
        if (p->end < D_0018CB20.time) {
            continue;
        }
        {
            short win[16];
            short w, h;
            int hw, hh;

            font_set_window(win, 0xC8, 0x208, 0x28, 0x1D8, 0x100, D_0013E500[1] - 0x38, 0x12,
                                7);
            font_print_window_regular(win, 0x80B0B0B0, D_0018CB20.subs + p->text[idx], -1);
            win[5] = D_0013E500[1] - 0x3C;
            w = win[6];
            h = win[7];
            hh = (h >> 1) + 5;
            hw = (w >> 1) + 10;
            if (win[5] + hh > D_0013E500[1] - 0x14) {
                win[5] = D_0013E500[1] - 0x14 - hh;
            }
            draw_ui_frame(win[5] - hh, win[5] + hh, 0x100 - hw, 0x100 + hw, 0x60);
            win[9] &= ~4;
            font_print_window_regular(win, 0x80B0B0B0, D_0018CB20.subs + p->text[idx], -1);
            return;
        }
    }
}

extern __typeof__(draw_subtitles) func_001F4BE0 __attribute__((alias("FUN_001f4be0")));
