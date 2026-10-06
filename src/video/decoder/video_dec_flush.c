#include "types.h"
struct Code4 {
    u8 b[4];
};
struct VideoDec {
    u8 pad0[0x48];
    u8 stream[0x60];
    s32 state;
};
extern struct Code4 D_00161218[];
extern s32 D_0016120C;
extern void video_dec_begin_put(struct VideoDec *, u32 *, s32 *, u32 *,
                                s32 *) __asm__("func_0023CBF0");
extern s32 cpy2area(u32, s32, u32, s32, void *, s32, s32,
                                    s32) __asm__("func_0023B810");
extern void video_dec_end_put(s32, s32) __asm__("func_0023CC10");
extern void vi_buf_flush(void *) __asm__("func_0023C660");
s32 video_dec_flush(struct VideoDec *vd) __asm__("FUN_0023cd08");

s32 video_dec_flush(struct VideoDec *vd) {
    struct Code4 code = D_00161218[0];
    u32 p0;
    s32 n0;
    u32 p1;
    s32 n1;
    s32 r;

    video_dec_begin_put(vd, &p0, &n0, &p1, &n1);
    if (n0 + n1 < 4) {
        return 0;
    }
    r = cpy2area((p0 & 0x0FFFFFFF) | 0x20000000, n0, (p1 & 0x0FFFFFFF) | 0x20000000,
                                 n1, &code, 4, 0, 0);
    video_dec_end_put(D_0016120C + 0xD9048, r);
    vi_buf_flush(vd->stream);
    if (vd->state == 0) {
        vd->state = 2;
    }
    return 1;
}

extern __typeof__(video_dec_flush) func_0023CD08 __attribute__((alias("FUN_0023cd08")));
