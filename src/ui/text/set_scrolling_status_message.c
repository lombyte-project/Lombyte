#include "types.h"

struct ScrollingStatus {
    u8  pad_0[0x2C];
    s32 unk2C;
    u8  pad_30[0x14];
    s32 unk44;
};

extern struct ScrollingStatus D_001E63C0;
extern u8                     scrolling_status_message_format[] __asm__("D_001E8A70");
extern s32                    sprintf();
void                          set_scrolling_status_message(s32 message) __asm__("FUN_00237e90");

void set_scrolling_status_message(s32 message) {
    sprintf(D_001E63C0.unk2C, scrolling_status_message_format, message);
    D_001E63C0.unk44 = 0;
}

extern void func_00237E90(s32 arg0) __attribute__((alias("FUN_00237e90")));
