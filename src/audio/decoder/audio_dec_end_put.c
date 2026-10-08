#include "types.h"
#include "rnc/audio/decoder/audio_dec.h"
void audio_dec_end_put(struct AudioDec *ad, s32 n) __asm__("FUN_0023ae28");

void audio_dec_end_put(struct AudioDec *ad, s32 n) {
    s32 k;
    s32 room;

    if (ad->state == 0) {
        if (ad->strType != 4) {
            room = 40 - ad->hdrCount;
            k = room < n ? room : n;
            ad->hdrCount += k;
            if (ad->hdrCount >= 40) {
                ad->state = 1;
            }
            n -= k;
        } else {
            ad->state = 1;
        }
    }
    ad->size = ad->size / 1024 * 1024;
    ad->put = (ad->put + n) % ad->size;
    ad->count += n;
    ad->totalBytes += n;
}

extern __typeof__(audio_dec_end_put) func_0023AE28 __attribute__((alias("FUN_0023ae28")));
