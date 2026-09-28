#include "types.h"
struct AudioDec { s32 done; s32 state; u8 pad8[0x28]; s32 fill; u8 pad34[4]; s32 pos; s32 total; s32 size; s32 count; };
void audio_dec_end_put(struct AudioDec *ad, s32 n) __asm__("FUN_0023ae28");

void audio_dec_end_put(struct AudioDec *ad, s32 n) {
    s32 k;
    s32 room;

    if (ad->done == 0) {
        if (ad->state != 4) {
            room = 40 - ad->fill;
            k = room < n ? room : n;
            ad->fill += k;
            if (ad->fill >= 40) {
                ad->done = 1;
            }
            n -= k;
        } else {
            ad->done = 1;
        }
    }
    ad->size = ad->size / 1024 * 1024;
    ad->pos = (ad->pos + n) % ad->size;
    ad->total += n;
    ad->count += n;
}

extern __typeof__(audio_dec_end_put) func_0023AE28 __attribute__((alias("FUN_0023ae28")));
