#include "types.h"
extern s32 snd_batch_index __asm__("D_0015ECC0");
extern s32 *snd_batch_command_buffers[2] __asm__("D_0015ECA0");
extern s32 snd_batch_free_bytes[2] __asm__("D_0015ECA8");
extern void *snd_batch_return_buffers[2] __asm__("D_0015ECB8");
extern u8 D_0015EBC0[];
extern char snd_non_idle_error_string[] __asm__("D_00153D20");
extern void StoreObjectIndex(void *, s32, s32 *);
extern s32 SceSifCheckStatRpc(void *);
extern s32 printf(const char *, ...);
extern void FlushCache(s32);
extern s32 sceSifCallRpc(void *, u32, u32, void *, s32, void *, s32, void (*)(void *), void *);
void snd_send_current_batch(void) __asm__("FUN_0012e9d8");

void snd_send_current_batch(void) {
    s32 next;

    StoreObjectIndex(snd_batch_return_buffers[snd_batch_index], *snd_batch_command_buffers[snd_batch_index], snd_batch_command_buffers[snd_batch_index]);
    while (SceSifCheckStatRpc(D_0015EBC0) != 0) {
        printf(snd_non_idle_error_string);
        FlushCache(0);
    }
    sceSifCallRpc(D_0015EBC0, 0x4D, 1, snd_batch_command_buffers[snd_batch_index], 0x1000 - snd_batch_free_bytes[snd_batch_index],
                  snd_batch_return_buffers[snd_batch_index], (*snd_batch_command_buffers[snd_batch_index] << 2) + 8, 0, 0);
    next = snd_batch_index != 1;
    snd_batch_index = next;
    *snd_batch_command_buffers[next] = 0;
    snd_batch_free_bytes[next] = 0xFFC;
}

extern __typeof__(snd_send_current_batch) func_0012E9D8 __attribute__((alias("FUN_0012e9d8")));
