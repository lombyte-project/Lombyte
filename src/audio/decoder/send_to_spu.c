#include "types.h"

extern void FlushCache(s32);
extern s32 sceSifSetDma(void *, s32);
extern s32 sceSifDmaStat(s32);
extern void snd_update_movie_adpcm(s32, s32) __asm__("FUN_0012f148");

void send_to_spu(s32 *arg0, s32 arg1, s32 arg2, s32 arg3) __asm__("FUN_0023af18");

void send_to_spu(s32 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    u32 descriptor[4];
    s32 dma_id;

    FlushCache(0);
    descriptor[0] = arg1;
    descriptor[1] = arg0[0x48 / 4];
    descriptor[2] = arg2;
    descriptor[3] = 0;
    do {
        dma_id = sceSifSetDma(descriptor, 1);
    } while (dma_id == 0);
    while (sceSifDmaStat(dma_id) >= 0) {
    }
    snd_update_movie_adpcm(arg2, arg3);
}

extern __typeof__(send_to_spu) func_0023AF18 __attribute__((alias("FUN_0023af18")));
