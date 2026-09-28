#include "types.h"
#define DGET(a) (*(volatile u32 *)(a))
typedef struct {
    u8 pad0[0x1C]; u32 d4_madr; u32 d4_tadr; u32 d4_qwc; u32 d4_chcr;
    u32 d3_madr; u32 d3_qwc; u32 d3_chcr; u32 ipu_bp; u32 ipu_ctrl; s32 sema; s32 active;
} ViBuf;
extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern void set_dma_channel_4_control_register(s32) __asm__("FUN_0023bbb0");
extern void set_dma_channel_3_control_register(s32) __asm__("FUN_0023bb40");
s32 vi_buf_stop_dma(ViBuf *f) __asm__("FUN_0023c170");

s32 vi_buf_stop_dma(ViBuf *f) {
    WaitSema(f->sema);
    f->active = 0;
    set_dma_channel_4_control_register(5);
    f->d4_madr = DGET(0x1000B410);
    f->d4_tadr = DGET(0x1000B430);
    f->d4_qwc = DGET(0x1000B420);
    f->d4_chcr = DGET(0x1000B400);
    if (DGET(0x10002010) & 0xF0) {
        do {
        } while (DGET(0x10002010) & 0xF0);
    }
    set_dma_channel_3_control_register(0);
    f->d3_madr = DGET(0x1000B010);
    f->d3_qwc = DGET(0x1000B020);
    f->d3_chcr = DGET(0x1000B000);
    f->ipu_bp = DGET(0x10002020);
    f->ipu_ctrl = DGET(0x10002010);
    SignalSema(f->sema);
    return 1;
}

extern __typeof__(vi_buf_stop_dma) func_0023C170 __attribute__((alias("FUN_0023c170")));
