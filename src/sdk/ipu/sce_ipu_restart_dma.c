#include "types.h"
#include "rnc/sdk/dma.h"

/* sceIpuDmaEnv in the PS2 SDK <libipu.h> (ipuctrl at 0x20 is not read). */
struct sceIpuDmaEnv {
    s32 d4madr;
    s32 d4tadr;
    s32 d4qwc;
    s32 d4chcr;
    s32 d3madr;
    s32 d3qwc;
    s32 d3chcr;
    s32 ipubp;
};

void sceIpuRestartDMA(struct sceIpuDmaEnv *ctx) {
    u32 bits;
    u32 magic;
    u32 count;
    s32 input_dma_quadword_count;
    s32 input_dma_address;

    bits = (u32)ctx->ipubp;
    magic = bits & 0x7F;
    count = ((bits >> 16) & 3) + ((bits >> 8) & 0xF);
    input_dma_address = ctx->d4madr - count * 0x10;
    input_dma_quadword_count = ctx->d4qwc + count;
    if (ctx->d3madr != 0) {
        if (ctx->d3qwc != 0) {
            *(volatile u32 *)0x1000B010 = ctx->d3madr;
            *(volatile u32 *)0x1000B020 = ctx->d3qwc;
            SetD3Chcr(ctx->d3chcr | 0x100);
        }
    }
    while (*(volatile s32 *)0x10002010 < 0) {
    }
    *(volatile u32 *)0x10002000 = magic;
    while (*(volatile s32 *)0x10002010 < 0) {
    }
    if (input_dma_address != 0) {
        if (input_dma_quadword_count != 0) {
            *(volatile u32 *)0x1000B410 = input_dma_address;
            *(volatile u32 *)0x1000B430 = ctx->d4tadr;
            *(volatile u32 *)0x1000B420 = input_dma_quadword_count;
            SetD4Chcr(ctx->d4chcr | 0x100);
        }
    }
}
