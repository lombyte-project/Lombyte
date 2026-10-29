#include "types.h"
#include "rnc/video/decoder/vi_buf.h"
#define DGET(a) (*(volatile u32 *)(a))
#define BLOCK_IN_DMA_RANGE(i, start, n_dma, n) \
    ((((i) + (n) - (start)) % (n)) >= 0 && (((i) + (n) - (start)) % (n)) < (n_dma))
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern s32 get_fifo_index(struct ViBuf *, u32) __asm__("FUN_0023baf8");
extern void set_d3_chcr(u32) __asm__("FUN_0023bb40");
extern void set_d4_chcr(u32) __asm__("FUN_0023bbb0");
s32 vi_buf_restart_dma(struct ViBuf *f) __asm__("FUN_0023c280");

/* Backs channel 4 up by the qwords still in the IPU FIFO, restarts channels 3 and 4
   and the IPU bit position saved by vi_buf_stop_dma. */
s32 vi_buf_restart_dma(struct ViBuf *video_buffer) {
    s32 bp = video_buffer->ipu_bp & 0x7F;
    s32 fifo_qwords = ((video_buffer->ipu_bp >> 16) & 3) + ((video_buffer->ipu_bp >> 8) & 0xF);
    u32 madr = video_buffer->d4_madr - (fifo_qwords << 4);
    u32 qwc = video_buffer->d4_qwc + fifo_qwords;
    u32 chcr = video_buffer->d4_chcr | 0x100;
    u32 tadr = video_buffer->d4_tadr;

    WaitSema(video_buffer->sema);

    if (madr < (u32)video_buffer->data) {
        s32 tag_id;
        qwc = ((u32)video_buffer->data - madr) >> 4;
        tadr = (u32)video_buffer->tag & 0x0FFFFFFF;
        madr += video_buffer->n << 11;
        tag_id = (video_buffer->d4_madr != (u32)video_buffer->data && video_buffer->d4_madr != (u32)video_buffer->data + (video_buffer->n << 11)) ? 3 : 0;
        chcr = (video_buffer->d4_chcr & 0x0FFFFFFF) | (tag_id << 28) | 0x100;
        if (!BLOCK_IN_DMA_RANGE(0, video_buffer->dma_start, video_buffer->dma_n, video_buffer->n)) {
            video_buffer->dma_start = video_buffer->n - 1;
            video_buffer->dma_n++;
        }
    } else {
        s32 saved_block = get_fifo_index(video_buffer, video_buffer->d4_madr);
        s32 block = get_fifo_index(video_buffer, madr);
        if (saved_block != block) {
            s32 tag_id;
            s32 size = video_buffer->n << 11;
            qwc = ((u32)video_buffer->data + (saved_block << 11) - madr) >> 4;
            tadr = ((saved_block << 4) + (u32)video_buffer->tag) & 0x0FFFFFFF;
            tag_id = ((u32)video_buffer->data + (video_buffer->d4_madr - (u32)video_buffer->data) % size
                      == (u32)video_buffer->data + (((video_buffer->dma_start + video_buffer->dma_n) % video_buffer->n) << 11)) ? 0 : 3;
            chcr = (video_buffer->d4_chcr & 0x0FFFFFFF) | (tag_id << 28) | 0x100;
            if (!BLOCK_IN_DMA_RANGE(block, video_buffer->dma_start, video_buffer->dma_n, video_buffer->n)) {
                video_buffer->dma_start = block;
                video_buffer->dma_n++;
            }
        }
    }

    if (video_buffer->d3_madr != 0 && video_buffer->d3_qwc != 0) {
        DGET(0x1000B010) = video_buffer->d3_madr;
        DGET(0x1000B020) = video_buffer->d3_qwc;
        set_d3_chcr(video_buffer->d3_chcr | 0x100);
    }
    if (video_buffer->dma_n != 0) {
        for (;;) {
            if (*(volatile s32 *)0x10002010 >= 0) {
                break;
            }
        }
        DGET(0x10002000) = bp;
        for (;;) {
            if (*(volatile s32 *)0x10002010 >= 0) {
                break;
            }
        }
    }
    DGET(0x1000B410) = madr;
    DGET(0x1000B430) = tadr;
    DGET(0x1000B420) = qwc;
    if (video_buffer->dma_n != 0) {
        set_d4_chcr(chcr);
    }
    DGET(0x10002010) = video_buffer->ipu_ctrl;
    video_buffer->is_active = 1;
    SignalSema(video_buffer->sema);
    return 1;
}

extern __typeof__(vi_buf_restart_dma) func_0023C280 __attribute__((alias("FUN_0023c280")));
