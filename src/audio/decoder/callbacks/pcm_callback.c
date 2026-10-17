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
    void *ptr0;
    s32 len0;
    void *ptr1;
    s32 len1;
    u8 *ps;
    u8 *end;
    s32 len;
    s32 n;
    s32 ret;
    s32 rest;

    len = cb->len - 4;
    ps = cb->data + 4;
    end = ab->data + ab->size;
    if (ps >= end) {
        ps -= ab->size;
    }
    n = end - ps;
    if (len < n) {
        n = len;
    }
    rest = len - n;
    audio_dec_begin_put(D_0016120C + 0xD9100, &ptr0, &len0, &ptr1, &len1);
    ret = cpy2area(ptr0, len0, ptr1, len1, ps, n, ab, rest);
    audio_dec_end_put(D_0016120C + 0xD9100, ret);
    return ret > 0;
}

extern __typeof__(pcm_callback) func_0023B728 __attribute__((alias("FUN_0023b728")));
