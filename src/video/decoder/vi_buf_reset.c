#include "types.h"
#include "eetypes.h"
#define DPUT(a, v) (*(volatile u32 *)(a) = (v))
#define DMA_ADDR(p) ((u32)(p) & 0x0FFFFFFF)
typedef struct { s64 pts; s64 dts; s32 pos; s32 len; } TimeStamp;
typedef struct {
    u128 *data; u128 *tag; s32 n; s32 dmaStart; s32 dmaN; s32 readBytes; s32 buffSize; u8 pad1C[0x24];
    s32 sema; s32 isActive; s64 totalBytes; TimeStamp *ts; s32 n_ts; s32 count_ts; s32 wt_ts;
} ViBuf;
extern void func_0023BC20(u128 *, u32, s32, s32);
extern void set_dma_channel_4_control_register(s32) __asm__("FUN_0023bbb0");
s32 vi_buf_reset(ViBuf *f) __asm__("FUN_0023bcc0");

s32 vi_buf_reset(ViBuf *f) {
    s32 i;

    f->isActive = 1;
    f->dmaStart = 0;
    f->dmaN = 0;
    f->readBytes = 0;
    f->count_ts = 0;
    f->wt_ts = 0;
    for (i = 0; i < f->n_ts; i++) {
        f->ts[i].pts = -1;
        f->ts[i].dts = -1;
        f->ts[i].pos = 0;
        f->ts[i].len = 0;
    }
    for (i = 0; i < f->n; i++) {
        func_0023BC20(f->tag + i, DMA_ADDR((i << 11) + (u32)f->data), 3, 0x80);
    }
    func_0023BC20(f->tag + i, DMA_ADDR(f->tag), 2, 0);
    DPUT(0x1000B420, 0);
    DPUT(0x1000B410, DMA_ADDR(f->data));
    DPUT(0x1000B430, DMA_ADDR(f->tag));
    set_dma_channel_4_control_register(5);
    return 1;
}

extern __typeof__(vi_buf_reset) func_0023BCC0 __attribute__((alias("FUN_0023bcc0")));
