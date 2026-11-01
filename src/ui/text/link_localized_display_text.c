#include "types.h"
#include "rnc/data_labels.h"

#include "rnc/ui/help/window_state.h"

#include "rnc/ui/help/message_entry.h"

extern HelpWindowState   help_window_state __asm__("D_001996D0");
extern HelpMessageEntry *help_msgs __asm__("D_0015F6A0");

#include "rnc/rendering/screen.h"

extern void allocate_voice_for_bank_entry(s32, s32, s32) __asm__("func_0022DB10");
extern void font_set_window(u16 *, u16, u16, u16, u16, u16, u16, u16, u32);
extern void font_print_window_small(void *, u64, void *, s32) __asm__("func_001F75F0");

void link_localized_display_text(void) __asm__("FUN_001fdd58");

void link_localized_display_text(void) {
    /* text_window is unsigned (lhu) but w/h are signed: the narrowing gives the
       sll 16 / sra 17 shift-by-one retail has instead of a plain sra. */
    u16              text_window[16];
    HelpWindowState *window;
    void            *text;
    s32              screen_height;
    s32              preferred_center_y;
    short            measured_text_width, rendered_text_height;
    s32              padded_half_height;

    help_window_state.state = HELP_WINDOW_OPENING;
    help_window_state.state_timer = 0;
    if (D_0015EE1D != 0 || D_0015EE1C != 0) {
        allocate_voice_for_bank_entry(0, 1, 0);
    }
    window = &help_window_state;
    text = help_msgs[window->message_index].text;
    font_set_window(text_window, 0xF0, 0x1E0, 0x2C, 0x1D4, 0x100, 0x168, 0x10, 7);
    font_print_window_small(text_window, 0x80FFA888L, text, -1);
    screen_height = screen_extent.height;
    measured_text_width = text_window[6];
    rendered_text_height = text_window[7];
    padded_half_height = (rendered_text_height >> 1) + 5;
    preferred_center_y = screen_height - 0x3C;
    window->target_half_width = (measured_text_width >> 1) + 10;
    window->target_half_height = padded_half_height;
    window->center_x = 0x100;
    window->frame_half_width = 8;
    window->frame_half_height = 8;
    window->center_y = preferred_center_y;
    if (screen_height - 0xC < preferred_center_y + padded_half_height) {
        window->center_y = screen_height - 0xC - padded_half_height;
    }
}

extern __typeof__(link_localized_display_text) func_001FDD58 __attribute__((alias("FUN_001fdd58")));
