#include "types.h"

struct VdecCb {
    s32 type;
    s32 pad4;
    u8 *data;
    s32 len;
    s64 pts;
    s64 dts;
};
struct VdecBuf {
    u8 data[0x50008];
    s32 size;
};

extern u8 *D_0016120C;
extern u8 D_001E8B08[];
extern void err_message(u8 *) __asm__("func_0023AB78");
extern s32 cpy2area(u32, s32, u32, s32, u8 *, s32, struct VdecBuf *,
                                    s32) __asm__("func_0023B810");
extern void video_dec_begin_put(u8 *, u32 *, s32 *, u32 *, s32 *) __asm__("func_0023CBF0");
extern void video_dec_end_put(u8 *, s32) __asm__("func_0023CC10");
extern s32 video_dec_put_ts(u8 *, s64, s64, u32, s32) __asm__("func_0023CC98");

s32 video_callback(s32 arg0, struct VdecCb *cb, struct VdecBuf *ab) __asm__("FUN_0023b5f0");

s32 video_callback(s32 arg0, struct VdecCb *cb, struct VdecBuf *ab) {
    u32 ptr0;
    s32 len0;
    u32 ptr1;
    s32 len1;
    u8 *ps;
    u8 *end;
    s32 len;
    s32 n;
    s32 rest;
    s32 ret;

    len = cb->len;
    ps = cb->data;
    end = ab->data + ab->size;
    n = end - ps;
    if (len < n) {
        n = len;
    }
    rest = len - n;
    video_dec_begin_put(D_0016120C + 0xD9048, &ptr0, &len0, &ptr1, &len1);
    ret = cpy2area((ptr0 & 0x0FFFFFFF) | 0x20000000, len0,
                                   (ptr1 & 0x0FFFFFFF) | 0x20000000, len1, ps, n, ab, rest);
    if (ret > 0) {
        if (video_dec_put_ts(D_0016120C + 0xD9048, cb->pts, cb->dts, ptr0, ret) == 0) {
            err_message(D_001E8B08);
        }
    }
    video_dec_end_put(D_0016120C + 0xD9048, ret);
    return ret > 0;
}

extern __typeof__(video_callback) func_0023B5F0 __attribute__((alias("FUN_0023b5f0")));
