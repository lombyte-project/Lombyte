#include "types.h"
#include "asm.h"

#include "types.h"
#include "sda.h"

struct SifClientDataStartSound {
    u8 pad_0[0x24];
    void *volatile server;
};

struct StartSoundWork {
    volatile s32 read_active;
    u8 pad_4[0xC];
    volatile s32 read_error;
};

extern u8 D_00133280[];
extern u8 D_00134280[];
extern u8 D_00135280[];
extern u8 D_00136280[];
extern u8 D_00137280[];
extern u8 D_001376C0[];
extern u8 D_00153C50[];
extern u8 D_00153C78[];
extern struct SifClientDataStartSound sound_command_client __asm__("D_0015EBC0");
extern struct SifClientDataStartSound sound_stream_client __asm__("D_0015EBE8");
extern struct StartSoundWork sound_read_work __asm__("D_00137B00");
extern u8 *D_0015ECA0[2] __attribute__((sda));
extern s32 D_0015ECA8 __attribute__((sda));
extern s32 D_0015ECAC MACRO_ADDR;
extern u8 *D_0015ECB0[2] __attribute__((sda));
extern u8 *D_0015ECB8[2] __attribute__((sda));
extern s32 D_0015ECC8 __attribute__((sda));
extern s32 D_0015ECD0 __attribute__((sda));
extern s64 D_0015ECD8 MACRO_ADDR;
extern s32 D_0015ED00 MACRO_ADDR;

extern void sceSifInitRpc(u32);
extern s32 sceSifBindRpc(struct SifClientDataStartSound *, u32, s32);
extern s32 printf(const char *, ...);
extern s32 snd_send_iop_command_and_wait(s32, s32, void *) __asm__("func_0012E548");

s32 snd_start_sound_system(void) __asm__("FUN_0012da28");

s32 snd_start_sound_system(void) {
    s32 bind_result;
    s32 command_arg;
    u8 *const command_buffer1 = D_00134280;
    u8 *const command_buffer0 = D_00133280;

    D_0015ECA0[0] = command_buffer0;
    D_0015ECA0[1] = command_buffer1;
    D_0015ECB8[0] = D_00137280;
    D_0015ECB8[1] = D_001376C0;
    D_0015ECB0[0] = D_00135280;
    D_0015ECB0[1] = D_00136280;
    sceSifInitRpc(0);

    for (;;) {
        bind_result = sceSifBindRpc(&sound_command_client, 0x123456, 0);
        if (bind_result < 0) {
            printf((const char *)D_00153C50, D_00153C78, 0x73);
            for (;;) {
            }
        }
        command_arg = 10000;
        for (command_arg--; command_arg != -1; command_arg--) {
        }
        if (sound_command_client.server != NULL) {
            break;
        }
    }

    D_0015ECC8 = 0;
    D_0015ECD0 = 0;
    D_0015ECD8 = 0;
    D_0015ED00 = 0;

    do {
        bind_result = sceSifBindRpc(&sound_stream_client, 0x123457, 0);
        if (bind_result < 0) {
            printf((const char *)D_00153C50, D_00153C78, 0x88);
            for (;;) {
            }
        }
        command_arg = 10000;
        for (command_arg--; command_arg != -1; command_arg--) {
        }
    } while (sound_stream_client.server == NULL);

    *(s32 *)D_00133280 = 0;
    sound_read_work.read_active = 0;
    *(s32 *)D_00134280 = 0;
    sound_read_work.read_error = 0;
    D_0015ECA8 = 0xFFC;
    D_0015ECAC = 0xFFC;
    command_arg = (s32)(u32)&sound_read_work.read_active;
    return snd_send_iop_command_and_wait(0, 4, &command_arg);
}
