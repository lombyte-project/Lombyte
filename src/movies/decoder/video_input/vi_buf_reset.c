#include "types.h"
#include "rnc/movies/decoder/vi_buf.h"
#include "eetypes.h"
#define DPUT(a, v)  (*(volatile u32 *)(a) = (v))
#define DMA_ADDR(p) ((u32)(p) & 0x0FFFFFFF)
extern void sc_tag2(u128 *, u32, s32, s32) __asm__("func_0023BC20");
extern void set_d4_chcr(s32) __asm__("FUN_0023bbb0");
s32         vi_buf_reset(struct ViBuf *f) __asm__("FUN_0023bcc0");

s32 vi_buf_reset(struct ViBuf *video_input_buffer) {
    s32 i;

    video_input_buffer->is_active = 1;
    video_input_buffer->dma_start = 0;
    video_input_buffer->dma_n = 0;
    video_input_buffer->read_bytes = 0;
    video_input_buffer->count_ts = 0;
    video_input_buffer->wt_ts = 0;
    for (i = 0; i < video_input_buffer->n_ts; i++) {
        video_input_buffer->ts[i].pts = -1;
        video_input_buffer->ts[i].dts = -1;
        video_input_buffer->ts[i].pos = 0;
        video_input_buffer->ts[i].len = 0;
    }
    for (i = 0; i < video_input_buffer->n; i++) {
        sc_tag2(video_input_buffer->tag + i,
                DMA_ADDR((i << 11) + (u32)video_input_buffer->data),
                3,
                0x80);
    }
    sc_tag2(video_input_buffer->tag + i, DMA_ADDR(video_input_buffer->tag), 2, 0);
    DPUT(0x1000B420, 0);
    DPUT(0x1000B410, DMA_ADDR(video_input_buffer->data));
    DPUT(0x1000B430, DMA_ADDR(video_input_buffer->tag));
    set_d4_chcr(5);
    return 1;
}

extern __typeof__(vi_buf_reset) func_0023BCC0 __attribute__((alias("FUN_0023bcc0")));
