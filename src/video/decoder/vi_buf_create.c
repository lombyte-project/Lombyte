#include "types.h"
#include "eetypes.h"

/* viBufCreate(ViBuf *, u128 *, u128 *, int, TimeStamp *, int) */

typedef struct { s64 pts; s64 dts; s32 pos; s32 len; } TimeStamp;
typedef struct {
    u128 *data; u128 *tag; s32 n; s32 dmaStart; s32 dmaN; s32 readBytes; s32 buffSize; u8 pad1C[0x24];
    s32 sema; s32 isActive; s64 totalBytes; TimeStamp *ts; s32 n_ts; s32 count_ts; s32 wt_ts;
} ViBuf;
typedef struct {
    s32 currentCount;
    s32 maxCount;
    s32 initCount;
    s32 numWaitThreads;
    u32 attr;
    u32 option;
} SemaParam;

#define VIBUF_ELM_SIZE 2048
#define UNCMASK 0x0FFFFFFF
#define UNCBASE 0x20000000

extern s32 CreateSema(SemaParam *);
s32 vi_buf_reset(ViBuf *f) __asm__("FUN_0023bcc0");

s32 vi_buf_create(ViBuf *f, u128 *data, u128 *tag, s32 size, TimeStamp *ts, s32 n_ts) __asm__("FUN_0023bc48");

s32 vi_buf_create(ViBuf *f, u128 *data, u128 *tag, s32 size, TimeStamp *ts, s32 n_ts) {
    SemaParam param;

    f->data = data;
    f->tag = (u128 *)((((u32)tag) & UNCMASK) | UNCBASE);
    f->n = size;
    f->buffSize = size * VIBUF_ELM_SIZE;

    f->ts = ts;
    f->n_ts = n_ts;

    param.initCount = 1;
    param.maxCount = 1;
    f->sema = CreateSema(&param);

    vi_buf_reset(f);

    f->totalBytes = 0;

    return 1;
}

extern __typeof__(vi_buf_create) func_0023BC48 __attribute__((alias("FUN_0023bc48")));
