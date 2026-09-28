#include "types.h"
#include "eetypes.h"
typedef struct { s64 pts; s64 dts; s32 pos; s32 len; } TimeStamp;
typedef struct {
    u128 *data; u128 *tag; s32 n; s32 dmaStart; s32 dmaN; s32 readBytes; s32 buffSize; u8 pad1C[0x24];
    s32 sema; s32 isActive; s64 totalBytes; TimeStamp *ts; s32 n_ts; s32 count_ts; s32 wt_ts;
} ViBuf;
static inline s32 IsInRegion(s32 tgt, s32 start, s32 len, s32 size) {
    return (tgt + size - start) % size < len;
}

s32 vi_buf_modify_pts(ViBuf *f, TimeStamp *new_ts) __asm__("FUN_0023c6b8");

s32 vi_buf_modify_pts(ViBuf *f, TimeStamp *new_ts) {
    TimeStamp *ts;
    s32 rd;
    s32 datasize;
    s32 loop;
    s32 len;

    rd = (f->wt_ts - f->count_ts + f->n_ts) % f->n_ts;
    datasize = f->n << 11;
    loop = 1;
    if (f->count_ts > 0) {
    while (loop) {
        ts = f->ts + rd;
        if (ts->len == 0 || new_ts->len == 0) {
            break;
        }
        if (IsInRegion(ts->pos, new_ts->pos, new_ts->len, datasize)) {
            len = new_ts->pos + new_ts->len - ts->pos;
            if (ts->len < len) {
                len = ts->len;
            }
            ts->pos = (ts->pos + len) % datasize;
            ts->len -= len;
            if (ts->len == 0) {
                if (ts->pts >= 0) {
                    ts->pts = -1;
                    ts->dts = -1;
                    ts->pos = 0;
                    ts->len = 0;
                }
                f->count_ts = f->count_ts - 1 > -1 ? f->count_ts - 1 : 0;
            }
        } else {
            loop = 0;
        }
        rd = (rd + 1) % f->n_ts;
    }
    }
    return 0;
}

extern __typeof__(vi_buf_modify_pts) func_0023C6B8 __attribute__((alias("FUN_0023c6b8")));
