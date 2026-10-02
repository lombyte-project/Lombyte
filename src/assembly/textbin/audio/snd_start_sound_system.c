#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/audio/snd_start_sound_system/FUN_0012da28.s", FUN_0012da28);
#else
#include "types.h"
#include "sda.h"

struct SifClientDataStartSound {
    u8 pad_0[0x24];
    s32 status;
};

struct StartSoundWork {
    s32 unk0;
    u8 pad_4[0xC];
    s32 unk10;
};

extern u8 D_00133280[];
extern u8 D_00134280[];
extern u8 D_00135280[];
extern u8 D_00136280[];
extern u8 D_00137280[];
extern u8 D_001376C0[];
extern struct StartSoundWork D_00137B00;
extern u8 D_00153C50[];
extern u8 D_00153C78[];
extern struct SifClientDataStartSound D_0015EBC0;
extern struct SifClientDataStartSound D_0015EBE8;
extern s32 D_0015ECA0 __attribute__((sda));
extern s32 D_0015ECA4 MACRO_ADDR;
extern s32 D_0015ECA8 __attribute__((sda));
extern s32 D_0015ECAC MACRO_ADDR;
extern s32 D_0015ECB0 __attribute__((sda));
extern s32 D_0015ECB4 MACRO_ADDR;
extern s32 D_0015ECB8 __attribute__((sda));
extern s32 D_0015ECBC MACRO_ADDR;
extern s32 D_0015ECC8 __attribute__((sda));
extern s32 D_0015ECD0 __attribute__((sda));
extern s64 D_0015ECD8 MACRO_ADDR;
extern s32 D_0015ED00 MACRO_ADDR;


extern void sceSifInitRpc(u32);
extern s32 sceSifBindRpc(struct SifClientDataStartSound *, u32, s32);
extern s32 printf(const char *, ...);
extern s32 func_0012E548(s32, s32, void *);

void snd_start_sound_system(void) __asm__("FUN_0012da28");

void snd_start_sound_system(void) {
    s32 ret;
    s32 wait;
    s32 command_arg;

    D_0015ECA0 = (s32)(u32)D_00133280;
    D_0015ECA4 = (s32)(u32)D_00134280;
    D_0015ECBC = (s32)(u32)D_001376C0;
    D_0015ECB8 = (s32)(u32)D_00137280;
    D_0015ECB4 = (s32)(u32)D_00136280;
    D_0015ECB0 = (s32)(u32)D_00135280;
    sceSifInitRpc(0);

    for (;;) {
        ret = sceSifBindRpc(&D_0015EBC0, 0x123456, 0);
        if (ret < 0) {
            printf((const char *)D_00153C50, D_00153C78, 0x73);
            for (;;) {
            }
        }
        command_arg = 10000;
        for (command_arg--; command_arg != -1; command_arg--) {
        }
        if (D_0015EBC0.status != 0) {
            break;
        }
    }

    D_0015ECC8 = 0;
    D_0015ECD0 = 0;
    D_0015ECD8 = 0;
    D_0015ED00 = 0;

    for (;;) {
        ret = sceSifBindRpc(&D_0015EBE8, 0x123457, 0);
        if (ret < 0) {
            printf((const char *)D_00153C50, D_00153C78, 0x88);
            for (;;) {
            }
        }
        command_arg = 10000;
        for (command_arg--; command_arg != -1; command_arg--) {
        }
        if (D_0015EBE8.status != 0) {
            break;
        }
    }

    *(s32 *)D_00133280 = 0;
    *(s32 *)D_00134280 = 0;
    D_00137B00.unk0 = 0;
    D_00137B00.unk10 = 0;
    D_0015ECA8 = 0xFFC;
    D_0015ECAC = 0xFFC;
    command_arg = (s32)(u32)&D_00137B00;
    func_0012E548(0, 4, &command_arg);
}
#endif /* NON_MATCHING */
