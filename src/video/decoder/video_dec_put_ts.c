#include "types.h"
#include "rnc/video/decoder/video_dec.h"

extern s32 D_0016120C;
extern s32 vi_buf_put_ts(s32, struct ViBufTimeStamp *) __asm__("func_0023C810");
/* Retail 0x0023cc98 passes a 0x18-byte timestamp record (PTS/DTS at
   +0/+8, relative position/length at +0x10/+0x14) to the global ViBuf at
   decoder context +0xD9090. Position subtraction wraps at 32 bits, as
   in EE subu. The helper's v0 survives the epilogue: return its result.
   vi_buf_put_ts returns 0 for a full timestamp queue, otherwise 1,
   including when both timestamps are negative and no entry is added.
   video_callback tests this result and reports the zero case. */
s32 video_dec_put_ts(struct VideoDec *vd, s64 pts, s64 dts, s32 pos,
                      s32 len) __asm__("FUN_0023cc98");

s32 video_dec_put_ts(struct VideoDec *vd, s64 pts, s64 dts, s32 pos, s32 len) {
    struct ViBufTimeStamp ts;

    ts.pts = pts;
    ts.dts = dts;
    ts.pos = (s32)((u32)pos - (u32)vd->vi_buf.data);
    ts.len = len;
    return vi_buf_put_ts(D_0016120C + 0xD9090, &ts);
}

extern __typeof__(video_dec_put_ts) func_0023CC98 __attribute__((alias("FUN_0023cc98")));
