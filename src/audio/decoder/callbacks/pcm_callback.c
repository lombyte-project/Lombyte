#include "types.h"
struct CbDataStr {
    s32 type;
    s32 pad4;
    u8 *data;
    s32 len;
};
struct AudioBuf {
    u8 data[0x50008];
    s32 size;
};
extern u8 *D_0016120C;
extern void audio_dec_begin_put(void *, void **, s32 *, void **, s32 *) __asm__("func_0023AD58");
extern s32 cpy2area(void *, s32, void *, s32, u8 *, s32, struct AudioBuf *,
                                    s32) __asm__("func_0023B810");
extern void audio_dec_end_put(void *, s32) __asm__("func_0023AE28");
s32 pcm_callback(void *mp, struct CbDataStr *cb, struct AudioBuf *ab) __asm__("FUN_0023b728");

s32 pcm_callback(void *mp, struct CbDataStr *cb, struct AudioBuf *ab) {
    void *first_destination;
    s32 first_destination_byte_count;
    void *second_destination;
    s32 second_destination_byte_count;
    u8 *ps;
    u8 *end;
    s32 len;
    s32 first_source_byte_count;
    s32 copied_byte_count;
    s32 rest;

    len = cb->len - 4;
    ps = cb->data + 4;
    end = ab->data + ab->size;
    if (ps >= end) {
        ps -= ab->size;
    }
    first_source_byte_count = end - ps;
    if (len < first_source_byte_count) {
        first_source_byte_count = len;
    }
    rest = len - first_source_byte_count;
    audio_dec_begin_put(D_0016120C + 0xD9100, &first_destination, &first_destination_byte_count, &second_destination, &second_destination_byte_count);
    copied_byte_count = cpy2area(first_destination, first_destination_byte_count, second_destination, second_destination_byte_count, ps, first_source_byte_count, ab, rest);
    audio_dec_end_put(D_0016120C + 0xD9100, copied_byte_count);
    return copied_byte_count > 0;
}

extern __typeof__(pcm_callback) func_0023B728 __attribute__((alias("FUN_0023b728")));
