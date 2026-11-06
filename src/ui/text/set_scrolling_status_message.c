#include "types.h"
#include "rnc/sdk/libc/stdio.h"

struct ScrollingStatus {
    u8  pad_0[0x2C];
    s32 text;
    u8  pad_30[0x14];
    s32 scroll;
};

extern struct ScrollingStatus D_001E63C0;
extern const char             scrolling_status_message_format[] __asm__("D_001E8A70");
void                          set_scrolling_status_message(s32 message) __asm__("FUN_00237e90");

void set_scrolling_status_message(s32 message) {
    sprintf(D_001E63C0.text, scrolling_status_message_format, message);
    D_001E63C0.scroll = 0;
}

extern void func_00237E90(s32 arg0) __attribute__((alias("FUN_00237e90")));

/* Eighteen spaces of indent before the message. */
const char scrolling_status_message_format[] __asm__("D_001E8A70") = "                  %s";
