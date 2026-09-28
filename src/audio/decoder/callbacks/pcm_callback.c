#include "types.h"
struct CbDataStr { s32 type; s32 pad4; u8 *data; s32 len; };
struct AudioBuf { u8 data[0x50008]; s32 size; };
extern u8 *D_0016120C;
extern void func_0023AD58(void *, void **, s32 *, void **, s32 *);
extern s32 func_0023B810(void *, s32, void *, s32, u8 *, s32, struct AudioBuf *, s32);
extern void func_0023AE28(void *, s32);
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
    func_0023AD58(D_0016120C + 0xD9100, &ptr0, &len0, &ptr1, &len1);
    ret = func_0023B810(ptr0, len0, ptr1, len1, ps, n, ab, rest);
    func_0023AE28(D_0016120C + 0xD9100, ret);
    return ret > 0;
}

extern __typeof__(pcm_callback) func_0023B728 __attribute__((alias("FUN_0023b728")));
