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
s32 vi_buf_restart_dma(struct ViBuf *f) {
    s32 bp = f->ipu_bp & 0x7F;
    s32 fifo_qwords = ((f->ipu_bp >> 16) & 3) + ((f->ipu_bp >> 8) & 0xF);
    u32 madr = f->d4_madr - (fifo_qwords << 4);
    u32 qwc = f->d4_qwc + fifo_qwords;
    u32 chcr = f->d4_chcr | 0x100;
    u32 tadr = f->d4_tadr;

    WaitSema(f->sema);

    if (madr < (u32)f->data) {
        s32 tag_id;
        qwc = ((u32)f->data - madr) >> 4;
        tadr = (u32)f->tag & 0x0FFFFFFF;
        madr += f->n << 11;
        tag_id = (f->d4_madr != (u32)f->data && f->d4_madr != (u32)f->data + (f->n << 11)) ? 3 : 0;
        chcr = (f->d4_chcr & 0x0FFFFFFF) | (tag_id << 28) | 0x100;
        if (!BLOCK_IN_DMA_RANGE(0, f->dma_start, f->dma_n, f->n)) {
            f->dma_start = f->n - 1;
            f->dma_n++;
        }
    } else {
        s32 saved_block = get_fifo_index(f, f->d4_madr);
        s32 block = get_fifo_index(f, madr);
        if (saved_block != block) {
            s32 tag_id;
            s32 size = f->n << 11;
            qwc = ((u32)f->data + (saved_block << 11) - madr) >> 4;
            tadr = ((saved_block << 4) + (u32)f->tag) & 0x0FFFFFFF;
            tag_id = ((u32)f->data + (f->d4_madr - (u32)f->data) % size
                      == (u32)f->data + (((f->dma_start + f->dma_n) % f->n) << 11)) ? 0 : 3;
            chcr = (f->d4_chcr & 0x0FFFFFFF) | (tag_id << 28) | 0x100;
            if (!BLOCK_IN_DMA_RANGE(block, f->dma_start, f->dma_n, f->n)) {
                f->dma_start = block;
                f->dma_n++;
            }
        }
    }

    if (f->d3_madr != 0 && f->d3_qwc != 0) {
        DGET(0x1000B010) = f->d3_madr;
        DGET(0x1000B020) = f->d3_qwc;
        set_d3_chcr(f->d3_chcr | 0x100);
    }
    if (f->dma_n != 0) {
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
    if (f->dma_n != 0) {
        set_d4_chcr(chcr);
    }
    DGET(0x10002010) = f->ipu_ctrl;
    f->is_active = 1;
    SignalSema(f->sema);
    return 1;
}

extern __typeof__(vi_buf_restart_dma) func_0023C280 __attribute__((alias("FUN_0023c280")));
