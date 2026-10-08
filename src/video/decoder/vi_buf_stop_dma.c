#include "types.h"
#include "rnc/video/decoder/vi_buf.h"
#define DGET(a) (*(volatile u32 *)(a))
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern void set_d4_chcr(s32) __asm__("FUN_0023bbb0");
extern void set_d3_chcr(s32) __asm__("FUN_0023bb40");
s32 vi_buf_stop_dma(struct ViBuf *f) __asm__("FUN_0023c170");

s32 vi_buf_stop_dma(struct ViBuf *f) {
    WaitSema(f->sema);
    f->is_active = 0;
    set_d4_chcr(5);
    f->d4_madr = DGET(0x1000B410);
    f->d4_tadr = DGET(0x1000B430);
    f->d4_qwc = DGET(0x1000B420);
    f->d4_chcr = DGET(0x1000B400);
    if (DGET(0x10002010) & 0xF0) {
        do {
        } while (DGET(0x10002010) & 0xF0);
    }
    set_d3_chcr(0);
    f->d3_madr = DGET(0x1000B010);
    f->d3_qwc = DGET(0x1000B020);
    f->d3_chcr = DGET(0x1000B000);
    f->ipu_bp = DGET(0x10002020);
    f->ipu_ctrl = DGET(0x10002010);
    SignalSema(f->sema);
    return 1;
}

extern __typeof__(vi_buf_stop_dma) func_0023C170 __attribute__((alias("FUN_0023c170")));
