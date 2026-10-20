#include "types.h"
#include "rnc/video/decoder/vi_buf.h"
#define DGET(a) (*(volatile u32 *)(a))
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern void set_d4_chcr(s32) __asm__("FUN_0023bbb0");
extern void set_d3_chcr(s32) __asm__("FUN_0023bb40");
s32 vi_buf_stop_dma(struct ViBuf *f) __asm__("FUN_0023c170");

s32 vi_buf_stop_dma(struct ViBuf *vi_buf) {
    WaitSema(vi_buf->sema);
    vi_buf->is_active = 0;
    set_d4_chcr(5);
    vi_buf->d4_madr = DGET(0x1000B410);
    vi_buf->d4_tadr = DGET(0x1000B430);
    vi_buf->d4_qwc = DGET(0x1000B420);
    vi_buf->d4_chcr = DGET(0x1000B400);
    if (DGET(0x10002010) & 0xF0) {
        do {
        } while (DGET(0x10002010) & 0xF0);
    }
    set_d3_chcr(0);
    vi_buf->d3_madr = DGET(0x1000B010);
    vi_buf->d3_qwc = DGET(0x1000B020);
    vi_buf->d3_chcr = DGET(0x1000B000);
    vi_buf->ipu_bp = DGET(0x10002020);
    vi_buf->ipu_ctrl = DGET(0x10002010);
    SignalSema(vi_buf->sema);
    return 1;
}

extern __typeof__(vi_buf_stop_dma) func_0023C170 __attribute__((alias("FUN_0023c170")));
