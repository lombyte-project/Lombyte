#include "types.h"

typedef struct {
    int state;  /* 0x00 */
    int x04;    /* 0x04 */
    int pad[7]; /* 0x08 */
    int x24;    /* 0x24 */
    int x28;    /* 0x28 */
    int count;  /* 0x2C: entries in the D_0015F6A0 table */
} HelpState;

typedef struct {
    void *text;
    s32 pad[3];
} TextEntry;

extern HelpState D_001996D0;
extern u8 D_0015EE1C;
extern u8 D_0015EE1D;
extern TextEntry *D_0015F6A0;
#include "rnc/rendering/screen.h"
extern void allocate_voice_for_bank_entry(s32, s32, s32) __asm__("func_0022DB10");
extern void font_set_window(u16 *, u16, u16, u16, u16, u16, u16, u16, u32);
extern void font_print_window_small(void *, u64, void *, s32) __asm__("func_001F75F0");

void link_localized_display_text(void) __asm__("FUN_001fdd58");

void link_localized_display_text(void) {
    /* win is unsigned (lhu) but w/h are signed: the narrowing gives the
       sll 16 / sra 17 shift-by-one retail has instead of a plain sra. */
    u16 win[16];
    HelpState *box;
    void *text;
    s32 screenY;
    s32 y0;
    short w, h;
    s32 hHalf5;

    D_001996D0.state = 1;
    D_001996D0.x04 = 0;
    if (D_0015EE1D != 0 || D_0015EE1C != 0) {
        allocate_voice_for_bank_entry(0, 1, 0);
    }
    box = &D_001996D0;
    text = D_0015F6A0[box->pad[6]].text;
    font_set_window(win, 0xF0, 0x1E0, 0x2C, 0x1D4, 0x100, 0x168, 0x10, 7);
    font_print_window_small(win, 0x80FFA888L, text, -1);
    screenY = screen_extent.height;
    w = win[6];
    h = win[7];
    hHalf5 = (h >> 1) + 5;
    y0 = screenY - 0x3C;
    box->pad[0] = (w >> 1) + 10;
    box->pad[1] = hHalf5;
    box->pad[2] = 0x100;
    box->pad[4] = 8;
    box->pad[5] = 8;
    box->pad[3] = y0;
    if (screenY - 0xC < y0 + hHalf5) {
        box->pad[3] = screenY - 0xC - hHalf5;
    }
}

extern __typeof__(link_localized_display_text) func_001FDD58 __attribute__((alias("FUN_001fdd58")));
