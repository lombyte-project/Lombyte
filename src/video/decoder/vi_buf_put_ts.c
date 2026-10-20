#include "types.h"
#include "rnc/video/decoder/vi_buf.h"

extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern void vi_buf_modify_pts(struct ViBuf *, struct ViBufTimeStamp *) __asm__("func_0023C6B8");
s32 vi_buf_put_ts(struct ViBuf *f, struct ViBufTimeStamp *ts) __asm__("FUN_0023c810");

s32 vi_buf_put_ts(struct ViBuf *video_buffer, struct ViBufTimeStamp *ts) {
    s32 ret;

    ret = 0;
    WaitSema(video_buffer->sema);
    if (video_buffer->count_ts < video_buffer->n_ts) {
        vi_buf_modify_pts(video_buffer, ts);
        if (ts->pts >= 0 || ts->dts >= 0) {
            video_buffer->ts[video_buffer->wt_ts].pts = ts->pts;
            video_buffer->ts[video_buffer->wt_ts].dts = ts->dts;
            video_buffer->ts[video_buffer->wt_ts].pos = ts->pos;
            video_buffer->ts[video_buffer->wt_ts].len = ts->len;
            video_buffer->count_ts++;
            video_buffer->wt_ts = (video_buffer->wt_ts + 1) % video_buffer->n_ts;
        }
        ret = 1;
    }
    SignalSema(video_buffer->sema);
    return ret;
}

extern __typeof__(vi_buf_put_ts) func_0023C810 __attribute__((alias("FUN_0023c810")));
