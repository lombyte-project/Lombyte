#include "types.h"

extern void FlushCache(s32);
extern s32 sceSifSetDma(void *, s32);
extern s32 sceSifDmaStat(s32);
extern void snd_update_movie_adpcm(s32, s32) __asm__("FUN_0012f148");

void send_to_spu(s32 *dec, s32 src, s32 size, s32 arg3) __asm__("FUN_0023af18");

void send_to_spu(s32 *dec, s32 src, s32 size, s32 arg3) {
    u32 descriptor[4];
    s32 dma_id;

    FlushCache(0);
    descriptor[0] = src;
    descriptor[1] = dec[0x48 / 4];
    descriptor[2] = size;
    descriptor[3] = 0;
    do {
        dma_id = sceSifSetDma(descriptor, 1);
    } while (dma_id == 0);
    while (sceSifDmaStat(dma_id) >= 0) {
    }
    snd_update_movie_adpcm(size, arg3);
}

extern __typeof__(send_to_spu) func_0023AF18 __attribute__((alias("FUN_0023af18")));
