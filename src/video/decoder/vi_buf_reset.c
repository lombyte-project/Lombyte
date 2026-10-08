#include "types.h"
#include "rnc/video/decoder/vi_buf.h"
#include "eetypes.h"
#define DPUT(a, v)  (*(volatile u32 *)(a) = (v))
#define DMA_ADDR(p) ((u32)(p) & 0x0FFFFFFF)
extern void func_0023BC20(u128 *, u32, s32, s32);
extern void set_d4_chcr(s32) __asm__("FUN_0023bbb0");
s32 vi_buf_reset(struct ViBuf *f) __asm__("FUN_0023bcc0");

s32 vi_buf_reset(struct ViBuf *f) {
    s32 i;

    f->is_active = 1;
    f->dma_start = 0;
    f->dma_n = 0;
    f->read_bytes = 0;
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
    set_d4_chcr(5);
    return 1;
}

extern __typeof__(vi_buf_reset) func_0023BCC0 __attribute__((alias("FUN_0023bcc0")));
