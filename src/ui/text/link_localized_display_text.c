#include "types.h"
#include "rnc/data_labels.h"
#include "rnc/ui/text/font.h"

#include "rnc/ui/help/help_state.h"

#include "rnc/ui/help/message_entry.h"

extern HelpMessageEntry *help_msgs __asm__("D_0015F6A0");

#include "rnc/rendering/screen.h"

extern void allocate_voice_for_bank_entry(s32, s32, s32) __asm__("func_0022DB10");
extern void font_print_window_small(void *, u64, void *, s32) __asm__("func_001F75F0");

void link_localized_display_text(void) __asm__("FUN_001fdd58");

void link_localized_display_text(void) {
    /* win is unsigned (lhu) but w/h are signed: the narrowing gives the
       sll 16 / sra 17 shift-by-one retail has instead of a plain sra. */
    u16        win[16];
    HelpState *box;
    void      *text;
    s32        screenY;
    s32        y0;
    short      measured_text_width, rendered_text_height;
    s32        hHalf5;

    help_state.state = 1;
    help_state.timer = 0;
    if (D_0015EE1D != 0 || D_0015EE1C != 0) {
        allocate_voice_for_bank_entry(0, 1, 0);
    }
    box = &help_state;
    text = help_msgs[box->msg].text;
    font_set_window(win, 0xF0, 0x1E0, 0x2C, 0x1D4, 0x100, 0x168, 0x10, 7);
    font_print_window_small(win, 0x80FFA888L, text, -1);
    screenY = screen_extent.height;
    measured_text_width = win[6];
    rendered_text_height = win[7];
    hHalf5 = (rendered_text_height >> 1) + 5;
    y0 = screenY - 0x3C;
    box->half_width = (measured_text_width >> 1) + 10;
    box->half_height = hHalf5;
    box->center_x = 0x100;
    box->frame_half_width = 8;
    box->frame_half_height = 8;
    box->center_y = y0;
    if (screenY - 0xC < y0 + hHalf5) {
        box->center_y = screenY - 0xC - hHalf5;
    }
}

extern __typeof__(link_localized_display_text) func_001FDD58 __attribute__((alias("FUN_001fdd58")));
