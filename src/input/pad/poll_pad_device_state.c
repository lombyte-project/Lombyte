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

void poll_pad_device_state(struct PAD *p) {
    s32 v;
    s32 r;
    s32 n;
    s32 i;
    u8 buf[0x40];

    p->unk1A0 = 0;
    v = p->unk1B0;
    p->unk1B0 = 0;
    p->unk1AC = p->unk348;
    p->unk1BC = v;
    r = scePad2GetState(p->socket);
    p->unk19C = r;
    if (r == 1) {
        switch (p->unk198) {
        case 0: {
            n = scePad2GetButtonProfile(p->socket, buf);
            if (*(u32 *)buf == 0xFFFFFFFF) {
                p->unk1DC = 0x79;
            } else {
                p->unk1DC = 0;
            }
            if (n < 5) {
                for (i = 0; i < n; i++) {
                    p->unk180[i] = buf[i];
                }
                p->unk198 = 1;
            } else {
                p->unk198 = 2;
                n = 0;
            }
            for (i = n; i < 4; i++) {
                p->unk180[i] = buf[i];
            }
            n = sceVibGetProfile(p->socket, buf);
            if (n < 5) {
                for (i = 0; i < n; i++) {
                    p->unk184[i] = buf[i];
                }
            } else {
                n = 0;
            }
            for (i = n; i < 4; i++) {
                p->unk184[i] = buf[i];
            }
            clear_pad_input(p);
            break;
        }
        case 1:
            n = scePad2Read(p->socket, buf);
            process_pad_input(p, buf, n);
            break;
        case 2:
            clear_pad_input(p);
            break;
        }
    } else {
        clear_pad_input(p);
        p->unk198 = 0;
    }
    p->unk188 = 0;
    p->unk189 = 0;
}

extern __typeof__(poll_pad_device_state) func_002170C8 __attribute__((alias("FUN_002170c8")));
