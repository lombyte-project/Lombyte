#include "types.h"
#include "rnc/audio/decoder/audio_dec.h"

void audio_dec_end_put(struct AudioDec *ad, s32 n) __asm__("FUN_0023ae28");

void audio_dec_end_put(struct AudioDec *ad, s32 byte_count) {
    s32 k;
    s32 room;

    if (ad->state == 0) {
        if (ad->strType != 4) {
            room = 40 - ad->hdrCount;
            k = room < byte_count ? room : byte_count;
            ad->hdrCount += k;
            if (ad->hdrCount >= 40) {
                ad->state = 1;
            }
            byte_count -= k;
        } else {
            ad->state = 1;
        }
    }
    ad->size = ad->size / 1024 * 1024;
    ad->put = (ad->put + byte_count) % ad->size;
    ad->count += byte_count;
    ad->totalBytes += byte_count;
}

extern __typeof__(audio_dec_end_put) func_0023AE28 __attribute__((alias("FUN_0023ae28")));
