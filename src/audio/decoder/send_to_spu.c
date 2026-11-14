#include "types.h"
#include "rnc/audio/decoder/audio_dec.h"
#include "rnc/audio/movie.h"

extern void FlushCache(s32);
extern s32  sceSifSetDma(void *, s32);
extern s32  sceSifDmaStat(s32);

void send_to_spu(struct AudioDec *dec,
                 s32              adpcm_source_address,
                 s32              size,
                 s32              staging_byte_offset) __asm__("FUN_0023af18");

void send_to_spu(struct AudioDec *dec,
                 s32              adpcm_source_address,
                 s32              transfer_byte_count,

                 s32 staging_byte_offset) {
    u32 descriptor[4];
    s32 dma_id;

    FlushCache(0);
    descriptor[0] = adpcm_source_address;
    descriptor[1] = dec->iopBuff;
    descriptor[2] = transfer_byte_count;
    descriptor[3] = 0;
    do {
        dma_id = sceSifSetDma(descriptor, 1);
    } while (dma_id == 0);
    while (sceSifDmaStat(dma_id) >= 0) {
    }
    snd_update_movie_adpcm(transfer_byte_count, staging_byte_offset);
}

extern __typeof__(send_to_spu) func_0023AF18 __attribute__((alias("FUN_0023af18")));
