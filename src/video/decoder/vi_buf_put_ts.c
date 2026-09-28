#include "types.h"
typedef struct { s64 pts; s64 dts; s32 pos; s32 len; } TimeStamp;
typedef struct { u8 pad0[0x40]; s32 sema; u8 pad44[0xC]; TimeStamp *ts; s32 size_ts; s32 count_ts; s32 put_ts; } ViBuf;
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern void func_0023C6B8(ViBuf *, TimeStamp *);
s32 vi_buf_put_ts(ViBuf *f, TimeStamp *ts) __asm__("FUN_0023c810");

s32 vi_buf_put_ts(ViBuf *f, TimeStamp *ts) {
    s32 ret;

    ret = 0;
    WaitSema(f->sema);
    if (f->count_ts < f->size_ts) {
        func_0023C6B8(f, ts);
        if (ts->pts >= 0 || ts->dts >= 0) {
            f->ts[f->put_ts].pts = ts->pts;
            f->ts[f->put_ts].dts = ts->dts;
            f->ts[f->put_ts].pos = ts->pos;
            f->ts[f->put_ts].len = ts->len;
            f->count_ts++;
            f->put_ts = (f->put_ts + 1) % f->size_ts;
        }
        ret = 1;
    }
    SignalSema(f->sema);
    return ret;
}

extern __typeof__(vi_buf_put_ts) func_0023C810 __attribute__((alias("FUN_0023c810")));
