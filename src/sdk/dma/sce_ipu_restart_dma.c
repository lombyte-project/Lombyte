#include "types.h"
struct IpuContext {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
};
extern s32 SetD3Chcr();
extern s32 SetD4Chcr();
void sceIpuRestartDMA(struct IpuContext *ctx) {
    u32 bits;
    u32 magic;
    u32 count;
    s32 src;
    s32 dst;

    bits = (u32)ctx->unk1C;
    magic = bits & 0x7F;
    count = ((bits >> 16) & 3) + ((bits >> 8) & 0xF);
    dst = ctx->unk0 - count * 0x10;
    src = ctx->unk8 + count;
    if (ctx->unk10 != 0) {
        if (ctx->unk14 != 0) {
            *(volatile u32 *)0x1000B010 = ctx->unk10;
            *(volatile u32 *)0x1000B020 = ctx->unk14;
            SetD3Chcr(ctx->unk18 | 0x100);
        }
    }
    while (*(volatile s32 *)0x10002010 < 0) {
    }
    *(volatile u32 *)0x10002000 = magic;
    while (*(volatile s32 *)0x10002010 < 0) {
    }
    if (dst != 0) {
        if (src != 0) {
            *(volatile u32 *)0x1000B410 = dst;
            *(volatile u32 *)0x1000B430 = ctx->unk4;
            *(volatile u32 *)0x1000B420 = src;
            SetD4Chcr(ctx->unkC | 0x100);
        }
    }
}
