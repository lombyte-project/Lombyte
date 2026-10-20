#include "types.h"

struct PAD {
    u8 pad0[0x180];
    u8 unk180[4];
    u8 unk184[4];
    u8 unk188;
    u8 unk189;
    u8 pad18A[0xA];
    s32 socket;
    s32 unk198;
    s32 unk19C;
    s32 unk1A0;
    s32 unk1A4;
    s32 unk1A8;
    s32 unk1AC;
    s32 unk1B0;
    u8 pad1B4[4];
    s32 unk1B8;
    s32 unk1BC;
    s32 unk1C0;
    s32 unk1C4;
    s32 unk1C8;
    s32 unk1CC;
    s32 unk1D0;
    s32 unk1D4;
    s32 unk1D8;
    s32 unk1DC;
    u8 pad1E0[0x168];
    s32 unk348;
};

extern s32 scePad2GetState(s32 socket);
extern s32 scePad2GetButtonProfile(s32 socket, u8 *profile);
extern s32 sceVibGetProfile(s32 port, u8 *profile);
extern s32 scePad2Read(s32 socket, void *buf);
extern void clear_pad_input(struct PAD *p) __asm__("func_002172C0");
extern void process_pad_input(struct PAD *p, u8 *buf, s32 len) __asm__("func_00217328");

void poll_pad_device_state(struct PAD *p) __asm__("FUN_002170c8");

void poll_pad_device_state(struct PAD *pad_state) {
    enum {
        PAD_PROFILE_INITIALIZATION = 0,
        PAD_INPUT_POLLING = 1,
        PAD_BUTTON_PROFILE_TOO_LARGE = 2
    };
    s32 v;
    s32 r;
    s32 byte_count;
    s32 i;
    u8 buf[0x40];

    pad_state->unk1A0 = 0;
    v = pad_state->unk1B0;
    pad_state->unk1B0 = 0;
    pad_state->unk1AC = pad_state->unk348;
    pad_state->unk1BC = v;
    r = scePad2GetState(pad_state->socket);
    pad_state->unk19C = r;
    if (r == 1) {
        switch (pad_state->unk198) {
        case PAD_PROFILE_INITIALIZATION: {
            byte_count = scePad2GetButtonProfile(pad_state->socket, buf);
            if (*(u32 *)buf == 0xFFFFFFFF) {
                pad_state->unk1DC = 0x79;
            } else {
                pad_state->unk1DC = 0;
            }
            if (byte_count < 5) {
                for (i = 0; i < byte_count; i++) {
                    pad_state->unk180[i] = buf[i];
                }
                pad_state->unk198 = 1;
            } else {
                pad_state->unk198 = 2;
                byte_count = 0;
            }
            for (i = byte_count; i < 4; i++) {
                pad_state->unk180[i] = buf[i];
            }
            byte_count = sceVibGetProfile(pad_state->socket, buf);
            if (byte_count < 5) {
                for (i = 0; i < byte_count; i++) {
                    pad_state->unk184[i] = buf[i];
                }
            } else {
                byte_count = 0;
            }
            for (i = byte_count; i < 4; i++) {
                pad_state->unk184[i] = buf[i];
            }
            clear_pad_input(pad_state);
            break;
        }
        case PAD_INPUT_POLLING:
            byte_count = scePad2Read(pad_state->socket, buf);
            process_pad_input(pad_state, buf, byte_count);
            break;
        case PAD_BUTTON_PROFILE_TOO_LARGE:
            clear_pad_input(pad_state);
            break;
        }
    } else {
        clear_pad_input(pad_state);
        pad_state->unk198 = 0;
    }
    pad_state->unk188 = 0;
    pad_state->unk189 = 0;
}

extern __typeof__(poll_pad_device_state) func_002170C8 __attribute__((alias("FUN_002170c8")));
