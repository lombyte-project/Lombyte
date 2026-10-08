#ifndef LOMBYTE_RNC_INPUT_PAD_STATE_H
#define LOMBYTE_RNC_INPUT_PAD_STATE_H

#include "types.h"

/* Partial view of the controller state D_0013C940 (init_pads, FUN_00217048). */
struct PadState {
    u8 pad_0[0x194];
    s32 socket;                         /* 0x194: scePad2CreateSocket(&param, &D_0013C940) result, init_pads */
    s32 unk198;                         /* 0x198: zeroed by init_pads */
    s32 unk19C;                         /* 0x19C: zeroed by init_pads */
    s32 unk1A0;                         /* 0x1A0 */
    s32 unk1A4;                         /* 0x1A4: button mask; menus test 0x20/0x40, saving_data_menu reads it as pad_buttons */
    u8 pad_1A8[0xC];
    s32 unk1B4;                         /* 0x1B4: alternative pad_buttons source in saving_data_menu */
    u8 pad_1B8[0x8];
    u32 held;                           /* 0x1C0 */
    u32 pressed;                        /* 0x1C4: menu handlers test 0xD00 (accept) and 0x10 each frame */
};

#define PAD_STATE_OFFSET_CHECK(field, off) \
    typedef char pad_state_offset_check_##field[ \
        ((unsigned long)&((struct PadState *)0)->field == (off)) ? 1 : -1]
PAD_STATE_OFFSET_CHECK(socket, 0x194);
PAD_STATE_OFFSET_CHECK(unk1A4, 0x1A4);
PAD_STATE_OFFSET_CHECK(held, 0x1C0);
PAD_STATE_OFFSET_CHECK(pressed, 0x1C4);

#endif
