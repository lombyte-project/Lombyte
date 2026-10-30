#include "types.h"
#include "asm.h"

#include "types.h"
#include "sda.h"
#include "rnc/audio/sound_read_work.h"
#include "rnc/audio/snd_start_sound_system_globals.h"

struct SifClientDataStartSound {
    u8 pad_0[0x24];
    void *volatile server;
};

extern u8                             snd_sif_bind_rpc_error_string[] __asm__("D_00153C50");
extern u8                             snd_989_snd_source_file[] __asm__("D_00153C78");
extern struct SifClientDataStartSound sound_command_client __asm__("D_0015EBC0");
extern struct SifClientDataStartSound sound_stream_client __asm__("D_0015EBE8");
extern s32           *snd_batch_command_buffers[2] __asm__("D_0015ECA0") __attribute__((sda));
extern s32            snd_batch_free_bytes __asm__("D_0015ECA8") __attribute__((sda));
extern s32 D_0015ECAC MACRO_ADDR;
extern struct SndCommandCallback *snd_stream_buffers[2] __asm__("D_0015ECB0") __attribute__((sda));
extern s32 *snd_batch_return_buffers[2] __asm__("D_0015ECB8") __attribute__((sda));
extern s32  snd_cd_callback_pending __asm__("D_0015ECC8") __attribute__((sda));
extern s32  snd_cd_callback_fn __asm__("D_0015ECD0") __attribute__((sda));
extern s64  snd_cd_callback_data __asm__("D_0015ECD8") MACRO_ADDR;
extern u32  snd_cd_callback_arg __asm__("D_0015ED00") MACRO_ADDR;

extern void sceSifInitRpc(u32);
extern s32  sceSifBindRpc(struct SifClientDataStartSound *, u32, s32);
extern s32  printf(const char *, ...);
extern s32  snd_send_iop_command_and_wait(s32, s32, void *) __asm__("func_0012E548");

s32 snd_start_sound_system(void) __asm__("FUN_0012da28");

s32 snd_start_sound_system(void) {
    s32        bind_result;
    s32        command_arg;
    s32 *const command_buffer1 = snd_command_buffer2;
    s32 *const command_buffer0 = snd_command_buffer1;

    snd_batch_command_buffers[0] = command_buffer0;
    snd_batch_command_buffers[1] = command_buffer1;
    snd_batch_return_buffers[0] = snd_return_buffer1;
    snd_batch_return_buffers[1] = snd_return_buffer2;
    snd_stream_buffers[0] = snd_stream_buffer1;
    snd_stream_buffers[1] = snd_stream_buffer2;
    sceSifInitRpc(0);

    for (;;) {
        bind_result = sceSifBindRpc(&sound_command_client, 0x123456, 0);
        if (bind_result < 0) {
            printf((const char *)snd_sif_bind_rpc_error_string, snd_989_snd_source_file, 0x73);
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

    snd_cd_callback_pending = 0;
    snd_cd_callback_fn = 0;
    snd_cd_callback_data = 0;
    snd_cd_callback_arg = 0;

    do {
        bind_result = sceSifBindRpc(&sound_stream_client, 0x123457, 0);
        if (bind_result < 0) {
            printf((const char *)snd_sif_bind_rpc_error_string, snd_989_snd_source_file, 0x88);
            for (;;) {
            }
        }
        command_arg = 10000;
        for (command_arg--; command_arg != -1; command_arg--) {
        }
    } while (sound_stream_client.server == NULL);

    snd_command_buffer1[0] = 0;
    sound_read_work.read_active = 0;
    snd_command_buffer2[0] = 0;
    sound_read_work.read_error = 0;
    snd_batch_free_bytes = 0xFFC;
    D_0015ECAC = 0xFFC;
    command_arg = (s32)(u32)&sound_read_work.read_active;
    return snd_send_iop_command_and_wait(0, 4, &command_arg);
}

s32 snd_command_buffer1[1024] = {0};

s32 snd_command_buffer2[1024] = {0};

struct SndCommandCallback snd_stream_buffer1[SND_BATCH_MAX_COMMANDS] = {{0}};

struct SndCommandCallback snd_stream_buffer2[SND_BATCH_MAX_COMMANDS] = {{0}};

s32 snd_return_buffer1[SND_BATCH_MAX_COMMANDS + 2] __attribute__((aligned(64))) = {0};

s32 snd_return_buffer2[SND_BATCH_MAX_COMMANDS + 2] __attribute__((aligned(64))) = {0};
