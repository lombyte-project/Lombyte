/* Ported from rac1-decomp, the PAL decompilation (src/game/help.c, func_001FF1B0). */
#include "sda.h"
typedef struct {
    int state;    /* 0x00 */
    int x04;      /* 0x04 */
    int pad[7];   /* 0x08 */
    int x24;      /* 0x24 */
    int x28;      /* 0x28 */
    int count;    /* 0x2C: entries in the D_0015F6A0 table */
} HelpState;
extern char D_001996D0[];
typedef struct {
    char *text;   /* 0x0 */
    int id;       /* 0x4 */
    int unk_08;
    int unk_0C;
} HelpEntry;
extern HelpEntry *D_0015F6A0 MACRO_ADDR;
extern int D_0015EE1D MACRO_ADDR;
extern int D_0015EE1C MACRO_ADDR;
#define D_0015EF1D_b (*(unsigned char *)&D_0015EE1D)
#define D_0015EF1C_b (*(unsigned char *)&D_0015EE1C)
extern void InitializeDmaPacket(void *arg0, int a1, int a2, int a3, int a4, int a5,
                          int a6, int a7, int a8);
extern void func_001F75F0(void *a, long b, void *c, int d);
extern void func_001F5F18(int, int, int, int, int);
extern void func_001F5450(int, int, int, int, int, int, int, int, long, long);
extern long func_001F44B8(int);
/* Help_Draw: nothing unless a state is active, the help window is shown
   (+0x30) and one of the fade flags is set. Then, by state, and only while
   D_0015EE1D is set: the growing frame (1), the prompt (2), the frame
   shrinking into the prompt icon with its fading quad (3), the full
   frame and the entry text, fading in (4) or out (6) through the text
   colour's alpha (4, 5, 6), or the closing frame (7). The fade flag is
   re-read in each case, as retail's per-case andi shows. */
void draw_help(void) __asm__("FUN_001fe980");

void draw_help(void) {
    if (((HelpState *)D_001996D0)->state == 0) {
        return;
    }
    if (*(int *)(D_001996D0 + 0x30) == 0) {
        return;
    }
    if (D_0015EF1D_b == 0 && D_0015EF1C_b == 0) {
        return;
    }
    switch (((HelpState *)D_001996D0)->state) {
    case 1:
        if (D_0015EF1D_b) {
            int s = ((HelpState *)D_001996D0)->x04 * 4 + 8;

            ((HelpState *)D_001996D0)->pad[4] = s;
            ((HelpState *)D_001996D0)->pad[5] = s;
            func_001F5F18(((HelpState *)D_001996D0)->pad[3] - s,
                          ((HelpState *)D_001996D0)->pad[3] + s,
                          ((HelpState *)D_001996D0)->pad[2] - s,
                          ((HelpState *)D_001996D0)->pad[2] + s, 0x60);
        }
        break;
    case 2:
        if (D_0015EF1D_b) {
            FUN_001fe898();
        }
        break;
    case 3:
        if (D_0015EF1D_b) {
            int a = (((HelpState *)D_001996D0)->pad[0] - 0x20) * ((HelpState *)D_001996D0)->x04 / 8 + 0x20;
            int b = (((HelpState *)D_001996D0)->pad[1] - 0x20) * ((HelpState *)D_001996D0)->x04 / 8 + 0x20;
            int alpha;

            ((HelpState *)D_001996D0)->pad[4] = a;
            ((HelpState *)D_001996D0)->pad[5] = b;
            func_001F5F18(((HelpState *)D_001996D0)->pad[3] - b,
                          ((HelpState *)D_001996D0)->pad[3] + b,
                          ((HelpState *)D_001996D0)->pad[2] - a,
                          ((HelpState *)D_001996D0)->pad[2] + a, 0x60);
            alpha = (8 - ((HelpState *)D_001996D0)->x04) * 16;
            if (alpha < 0) {
                alpha = 0;
            }
            func_001F5450(((HelpState *)D_001996D0)->pad[2] - 0x20,
                          ((HelpState *)D_001996D0)->pad[3] - 0x20, 0x40, 0x40, 0, 0, 0x40, 0x40,
                          (alpha << 24) | 0x808080, func_001F44B8(4));
        }
        break;
    case 4:
    case 5:
    case 6:
        if (D_0015EF1D_b) {
            short win[12];
            int col = 0x80FFA888;
            char *text;

            ((HelpState *)D_001996D0)->pad[4] = ((HelpState *)D_001996D0)->pad[0];
            ((HelpState *)D_001996D0)->pad[5] = ((HelpState *)D_001996D0)->pad[1];
            func_001F5F18(((HelpState *)D_001996D0)->pad[3] - ((HelpState *)D_001996D0)->pad[1],
                          ((HelpState *)D_001996D0)->pad[3] + ((HelpState *)D_001996D0)->pad[1],
                          ((HelpState *)D_001996D0)->pad[2] - ((HelpState *)D_001996D0)->pad[0],
                          ((HelpState *)D_001996D0)->pad[2] + ((HelpState *)D_001996D0)->pad[0], 0x60);
            if (((HelpState *)D_001996D0)->state == 4) {
                col = 0xFFA888 | (((HelpState *)D_001996D0)->x04 << 29);
            } else if (((HelpState *)D_001996D0)->state == 6) {
                col = ((4 - ((HelpState *)D_001996D0)->x04) << 29) | 0xFFA888;
            }
            text = D_0015F6A0[((HelpState *)D_001996D0)->pad[6]].text;
            InitializeDmaPacket((void *)win, 0xF0, 0x1E0, 0x2C, 0x1D4, 0x100,
                          ((HelpState *)D_001996D0)->pad[3], 0x10, 3);
            func_001F75F0((void *)win, col, text, -1);
        }
        break;
    case 7:
        if (D_0015EF1D_b) {
            int b = ((HelpState *)D_001996D0)->pad[4]
                  - (((HelpState *)D_001996D0)->pad[4] - 8) * ((HelpState *)D_001996D0)->x04 / 8;
            int a = ((HelpState *)D_001996D0)->pad[5]
                  - (((HelpState *)D_001996D0)->pad[5] - 8) * ((HelpState *)D_001996D0)->x04 / 8;

            func_001F5F18(((HelpState *)D_001996D0)->pad[3] - a,
                          ((HelpState *)D_001996D0)->pad[3] + a,
                          ((HelpState *)D_001996D0)->pad[2] - b,
                          ((HelpState *)D_001996D0)->pad[2] + b,
                          (8 - ((HelpState *)D_001996D0)->x04) * 12);
        }
        break;
    }
}

extern __typeof__(draw_help) func_001FE980 __attribute__((alias("FUN_001fe980")));
