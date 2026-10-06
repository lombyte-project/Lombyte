#include "types.h"
struct TsEntry {
    s64 pts;
    s64 dts;
    s32 pos;
    s32 len;
};
struct VideoDec {
    u8 pad0[0x48];
    s32 base;
};
extern s32 D_0016120C;
extern void vi_buf_put_ts(s32, struct TsEntry *) __asm__("func_0023C810");
void video_dec_put_ts(struct VideoDec *vd, s64 pts, s64 dts, s32 pos,
                      s32 len) __asm__("FUN_0023cc98");

void video_dec_put_ts(struct VideoDec *vd, s64 pts, s64 dts, s32 pos, s32 len) {
    struct TsEntry ts;

    ts.pts = pts;
    ts.dts = dts;
    ts.pos = pos - vd->base;
    ts.len = len;
    vi_buf_put_ts(D_0016120C + 0xD9090, &ts);
}

extern __typeof__(video_dec_put_ts) func_0023CC98 __attribute__((alias("FUN_0023cc98")));
