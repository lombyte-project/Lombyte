#include "types.h"
#include "rnc/video/decoder/vi_buf.h"

extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern void vi_buf_modify_pts(struct ViBuf *, struct ViBufTimeStamp *) __asm__("func_0023C6B8");
s32 vi_buf_put_ts(struct ViBuf *f, struct ViBufTimeStamp *ts) __asm__("FUN_0023c810");

s32 vi_buf_put_ts(struct ViBuf *f, struct ViBufTimeStamp *ts) {
    s32 ret;

    ret = 0;
    WaitSema(f->sema);
    if (f->count_ts < f->n_ts) {
        vi_buf_modify_pts(f, ts);
        if (ts->pts >= 0 || ts->dts >= 0) {
            f->ts[f->wt_ts].pts = ts->pts;
            f->ts[f->wt_ts].dts = ts->dts;
            f->ts[f->wt_ts].pos = ts->pos;
            f->ts[f->wt_ts].len = ts->len;
            f->count_ts++;
            f->wt_ts = (f->wt_ts + 1) % f->n_ts;
        }
        ret = 1;
    }
    SignalSema(f->sema);
    return ret;
}

extern __typeof__(vi_buf_put_ts) func_0023C810 __attribute__((alias("FUN_0023c810")));
