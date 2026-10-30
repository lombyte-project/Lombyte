#include "types.h"
#include "kernel.h"
#include "eetypes.h"
#include "rnc/movies/decoder/video_input/vi_buf_reset.h"

/* viBufCreate(ViBuf *, u128 *, u128 *, int, TimeStamp *, int) */

typedef struct {
    s64 pts;
    s64 dts;
    s32 pos;
    s32 len;
} TimeStamp;

typedef struct {
    u128      *data;
    u128      *tag;
    s32        n;
    s32        dmaStart;
    s32        dmaN;
    s32        readBytes;
    s32        buffSize;
    u8         pad1C[0x24];
    s32        sema;
    s32        isActive;
    s64        totalBytes;
    TimeStamp *ts;
    s32        n_ts;
    s32        count_ts;
    s32        wt_ts;
} ViBuf;

#define VIBUF_ELM_SIZE 2048
#define UNCMASK        0x0FFFFFFF
#define UNCBASE        0x20000000

extern s32 CreateSema(struct SemaParam *);

s32 vi_buf_create(ViBuf *f, u128 *data, u128 *tag, s32 size, TimeStamp *ts, s32 n_ts) __asm__("FUN_0023bc48");

s32 vi_buf_create(

    ViBuf *video_input_buffer, u128 *data, u128 *tag, s32 size, TimeStamp *ts, s32 n_ts) {
    struct SemaParam param;

    video_input_buffer->data = data;
    video_input_buffer->tag = (u128 *)((((u32)tag) & UNCMASK) | UNCBASE);
    video_input_buffer->n = size;
    video_input_buffer->buffSize = size * VIBUF_ELM_SIZE;

    video_input_buffer->ts = ts;
    video_input_buffer->n_ts = n_ts;

    param.initCount = 1;
    param.maxCount = 1;
    video_input_buffer->sema = CreateSema(&param);

    vi_buf_reset(video_input_buffer);

    video_input_buffer->totalBytes = 0;

    return 1;
}

extern __typeof__(vi_buf_create) func_0023BC48 __attribute__((alias("FUN_0023bc48")));
