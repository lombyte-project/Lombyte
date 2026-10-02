#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/video/decoder/vi_buf_restart_dma/FUN_0023c280.s", FUN_0023c280);
#else
#include "types.h"

typedef struct {
    u32 data;
    u32 tag;
    s32 n;
    s32 dmaStart;
    s32 readBytes;
    u8 pad14[8];
    u32 d4_madr;
    u32 d4_tadr;
    u32 d4_qwc;
    u32 d4_chcr;
    u32 d3_madr;
    u32 d3_qwc;
    u32 d3_chcr;
    u32 ipu_bp;
    u32 ipu_ctrl;
    s32 sema;
    s32 active;
} ViBuf;

#define DGET(a) (*(volatile u32 *)(a))

extern s32 WaitSema(s32);
extern s32 SignalSema(s32);
extern s32 func_0023BAF8(ViBuf *, s32);
extern void func_0023BB40(s32);
extern void func_0023BBB0(s32);

s32 vi_buf_restart_dma(ViBuf *f) __asm__("FUN_0023c280");

s32 vi_buf_restart_dma(ViBuf *f) {
    s32 off;
    s32 ifc;
    s32 chcr;
    s32 dir;
    s32 r;
    s32 t;
    s32 u;
    s32 r1;
    s32 r2;
    u32 var_18;
    u32 var_20;
    u32 var_21;
    u32 var_19;
    u32 bp;

    ifc = (f->ipu_bp >> 8) & 0xF;
    off = (f->ipu_bp >> 16) & 3;
    off = off + ifc;
    var_18 = f->d4_madr - (off << 4);
    var_21 = f->d4_qwc + off;
    var_19 = f->d4_chcr | 0x100;
    var_20 = f->d4_tadr;
    bp = f->ipu_bp & 0x7F;

    WaitSema(f->sema);

    if (var_18 < f->data) {
        s32 datasize;
        datasize = f->n << 11;
        var_21 = (f->data - var_18) >> 4;
        var_20 = f->tag & 0x0FFFFFFF;
        var_18 = var_18 + datasize;
        r = 0;
        if (f->d4_madr != f->data) {
            r = 3;
            if ((f->d4_madr ^ (f->data + datasize)) == 0) {
                r = 0;
            }
        }
        r = r << 28;
        var_19 = (f->d4_chcr & 0x0FFFFFFF) | r | 0x100;
        datasize = f->n - f->dmaStart;
        if (datasize % f->n < 0 || datasize % f->n >= f->readBytes) {
            f->dmaStart = f->n - 1;
            f->readBytes = f->readBytes + 1;
        }
    } else {
        s32 datasize;
        r1 = func_0023BAF8(f, f->d4_madr);
        r2 = func_0023BAF8(f, var_18);
        if (r1 != r2) {
            dir = 3;
            datasize = f->n << 11;
            t = (f->d4_madr - f->data) % (u32)datasize;
            u = (f->dmaStart + f->readBytes) % f->n;
            if (((f->data + t) ^ (f->data + (u << 11))) == 0) {
                dir = 0;
            }
            chcr = f->d4_chcr & 0x0FFFFFFF;
            var_21 = (f->data + (r1 << 11) - var_18) >> 4;
            var_20 = ((r1 << 4) + f->tag) & 0x0FFFFFFF;
            dir = dir << 28;
            var_19 = chcr | dir | 0x100;
            r = ((r2 + f->n) - f->dmaStart) % f->n;
            if (r < 0 || (((r2 + f->n) - f->dmaStart) % f->n) >= f->readBytes) {
                f->readBytes = f->readBytes + 1;
                f->dmaStart = r2;
            }
        }
    }

    if (f->d3_madr != 0) {
        if (f->d3_qwc != 0) {
            DGET(0x1000B010) = f->d3_madr;
            DGET(0x1000B020) = f->d3_qwc;
            func_0023BB40(f->d3_chcr | 0x100);
        }
    }
    if (f->readBytes != 0) {
        if (*(volatile s32 *)0x10002010 < 0) {
            while (*(volatile s32 *)0x10002010 < 0) {
            }
        }
        DGET(0x10002000) = bp;
        if (*(volatile s32 *)0x10002010 < 0) {
            while (*(volatile s32 *)0x10002010 < 0) {
            }
        }
    }
    DGET(0x1000B410) = var_18;
    DGET(0x1000B430) = var_20;
    DGET(0x1000B420) = var_21;
    if (f->readBytes != 0) {
        func_0023BBB0(var_19);
    }
    DGET(0x10002010) = f->ipu_ctrl;
    f->active = 1;
    SignalSema(f->sema);
    return 1;
}
#endif /* NON_MATCHING */
