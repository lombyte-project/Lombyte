#ifndef LOMBYTE_RNC_INPUT_PAD_STATE_H
#define LOMBYTE_RNC_INPUT_PAD_STATE_H

#include "types.h"

/*
 * Controller state D_0013C940, polled each frame by poll_pad_device_state
 * (state machine on profile_state) and filled by process_pad_input
 * (FUN_00217328); socket set up by init_pads.
 *
 * Button masks: process_pad_input ORs left-stick directions into `held`
 * (0x8000/0x2000 x, 0x1000/0x4000 y) and swaps 0x8000/0x2000 when the
 * mirror flag D_0015EDB4 is set. The *_unmasked copies are taken before the
 * `mode` 1/2 masks and have that swap undone; menus read them.
 */
struct PadState {
    u8 pad_0[0x100] __attribute__((aligned(16)));
    f32 analog[16];                     /* 0x100: [0..3] stick axes (-1..1), [4..15] pressure (0..1) */
    f32 analog_prev[16];                /* 0x140: copy of analog */
    u8 pad_180[0x9];
    u8 unk189;                          /* 0x189: copied by FUN_L00_002d2ee8 on spawn */
    u8 pad_18A[0xA];
    s32 socket;                         /* 0x194: scePad2CreateSocket(&param, &D_0013C940) result, init_pads */
    s32 profile_state;                  /* 0x198: 0 query profile, 1 read pad, 2 profile too long */
    s32 device_state;                   /* 0x19C: scePad2GetState result */
    s32 held;                           /* 0x1A0: buttons down, plus stick directions */
    s32 pressed;                        /* 0x1A4: newly down this frame (~prev_held & held) */
    s32 released;                       /* 0x1A8: ~held & prev_held */
    s32 prev_held;                      /* 0x1AC */
    s32 raw_held;                       /* 0x1B0: buttons only, no stick directions */
    s32 raw_pressed;                    /* 0x1B4: ~prev_held & raw_held; saving_data_menu pad_buttons source */
    s32 raw_released;                   /* 0x1B8: ~held & prev_raw_held; weapon swap tests 0x10 */
    s32 prev_raw_held;                  /* 0x1BC: raw_held of the previous poll */
    u32 held_unmasked;                  /* 0x1C0 */
    u32 pressed_unmasked;               /* 0x1C4: menu handlers test 0xD00 (accept) and 0x10 each frame */
    u32 released_unmasked;              /* 0x1C8 */
    s32 mode;                           /* 0x1CC: 1 masks off 0x5030 and the left stick, 2 keeps only 0x900; reset to 0 */
    s32 no_buttons;                     /* 0x1D0: held == 0 */
    s32 no_direction;                   /* 0x1D4: (held & 0xF000) == 0 */
    s32 stick_moved;                    /* 0x1D8: analog[2] or analog[3] nonzero */
    s32 unk1DC;                         /* 0x1DC: 0x79 if the button profile is all ones, else 0; l01 picks pressure input on 0x79 */
};

/*
 * The same pad state as some functions read it: the stick axes, and the held/pressed button words,
 * which are also tested together as one doubleword (FUN_L00_00298f90, the debug camera's
 * two-button chords).
 */
struct PadStateWords {
    char pad0[0x100];
    f32 analog[4];          /* 0x100: stick axes */
    char pad110[0x1A0 - 0x110];
    union {
        u64 held_pressed;   /* 0x1A0: held | pressed << 32 */
        struct {
            s32 held;       /* 0x1A0 */
            s32 pressed;    /* 0x1A4 */
        } w;
    } buttons;
    char pad1A8[0x1D8 - 0x1A8];
    s32 stick_moved;        /* 0x1D8: analog[2] or analog[3] nonzero */
};

#endif
