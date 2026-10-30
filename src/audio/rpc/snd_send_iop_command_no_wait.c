#include "types.h"
#include "rnc/audio/rpc/snd_send_iop_command_no_wait_globals.h"

struct SndCb {
    s32 func;
    s32 pad;
    s64 data;
};

extern s32          *snd_current_buffer __asm__("D_0015EC80") __attribute__((sda));
extern s32           snd_batch_index __asm__("D_0015ECC0") __attribute__((sda));
extern s32          *snd_batch_command_buffers[2] __asm__("D_0015ECA0") __attribute__((sda));
extern s32           snd_batch_free_bytes[2] __asm__("D_0015ECA8") __attribute__((sda));
extern struct SndCb *snd_stream_buffers[2] __asm__("D_0015ECB0") __attribute__((sda));
extern u8            sound_command_client[] __asm__("D_0015EBC0");
extern char          snd_non_idle_error_string[] __asm__("D_00153D20");
extern char          snd_no_wait_continuing_string[] __asm__("D_00154070");
extern s32           printf(const char *, ...);
extern s32           snd_flush_sound_commands(void) __asm__("func_0012DC80");
extern void          snd_post_message(void) __asm__("func_0012E9A0");
extern s32           FlushCache(s32);
extern s32           StoreObjectIndex(void *, s32);
extern s32           SceSifCheckStatRpc(void *);
extern s32           sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);
void snd_send_iop_command_no_wait(s32 cmd, s32 size, u8 *data, s32 cb, s64 cb_data) __asm__("FUN_0012e6e0");

extern s32 GlobalTableStatus __asm__("D_0015ECC4") __attribute__((sda));

void snd_send_iop_command_no_wait(s32 cmd, s32 size, u8 *data, s32 cb, s64 cb_data) {
    s32 need;
    s32 tries;
    s32 held;
    s32 i;
    u8 *command_write_cursor;

    held = 0;
    if (GlobalTableStatus == 0 && snd_current_buffer == 0 && size == 0 && cb == 0) {
        StoreObjectIndex(snd_sync_buffer, 1);
        while (SceSifCheckStatRpc(sound_command_client) != 0) {
            printf(snd_non_idle_error_string);
            snd_flush_sound_commands();
            FlushCache(0);
        }
        sceSifCallRpc(sound_command_client, cmd, 1, 0, 0, snd_sync_buffer, 0xC, 0, 0);
        return;
    }
    need = size + 4;
    tries = 0;
    if (need % 4 != 0) {
        need += 4 - need % 4;
    }
    while (*snd_batch_command_buffers[snd_batch_index] == 0x100 ||
           snd_batch_free_bytes[snd_batch_index] < need) {
        if (GlobalTableStatus != 0) {
            GlobalTableStatus = 0;
            held = 1;
        }
        snd_flush_sound_commands();
        if (tries == 1) {
            printf(
                "snd_SendIOPCommandNoWait: BUFFER %d FULL(%d)! Sound stalled waiting for IOP comands to finish!\n",
                snd_batch_index,
                *snd_batch_command_buffers[snd_batch_index]);
        }
        tries++;
    }
    if (tries != 0) {
        printf(snd_no_wait_continuing_string, tries);
    }
    if (held != 0) {
        GlobalTableStatus = 1;
    }
    command_write_cursor = (u8 *)snd_batch_command_buffers[snd_batch_index] -
                           (snd_batch_free_bytes[snd_batch_index] - 0x1000);
    *(s16 *)command_write_cursor = cmd;
    command_write_cursor += 2;
    *(s16 *)command_write_cursor = size;
    command_write_cursor += 2;
    for (i = 0; i < size; i++) {
        command_write_cursor[i] = data[i];
    }
    snd_batch_free_bytes[snd_batch_index] -= need;
    snd_stream_buffers[snd_batch_index][*snd_batch_command_buffers[snd_batch_index]].func = cb;
    snd_stream_buffers[snd_batch_index][*snd_batch_command_buffers[snd_batch_index]].data = cb_data;
    snd_post_message();
}

extern __typeof__(snd_send_iop_command_no_wait) func_0012E6E0
    __attribute__((alias("FUN_0012e6e0")));

s32 snd_sync_buffer[4] = {0};
