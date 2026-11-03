#include "types.h"

typedef struct {
    u8    sts;
    u8    std;
    u8    mfd;
    u8    rcyc;
    u16   express;
    u16   notify;
    u16   sqwc;
    u16   tqwc;
    void *rbadr;
    u32   rbmsk;
} sceDmaEnv;

/* D_CTRL field codes by DMA channel (0 VIF0, 1 VIF1, 2 GIF, 3 fromIPU, 4 toIPU,
   5 SIF0, 6 SIF1, 7 SIF2, 8 fromSPR, 9 toSPR); 0 = the channel cannot take that role. */
/* STS (stall source): 1 SIF0, 2 fromSPR, 3 fromIPU. */
const u8 dma_sts_control_encodings[10] __asm__("D_00153420") = {0, 0, 0, 3, 0, 1, 0, 0, 2, 0};
/* STD (stall drain): 1 VIF1, 2 GIF, 3 SIF1. */
const u8 dma_std_control_encodings[10] __asm__("D_00153430") = {0, 1, 2, 0, 0, 0, 3, 0, 0, 0};
/* MFD (MFIFO drain): 2 VIF1, 3 GIF. */
const u8 dma_mfd_control_encodings[10] __asm__("D_00153440") = {0, 2, 3, 0, 0, 0, 0, 0, 0, 0};
extern sceDmaEnv dma_last_applied_environment __asm__("D_001599D8");

s32 sceDmaPutEnv(sceDmaEnv *env) {
    u32 ctrl;
    u32 pcr;
    u32 sqwc;
    u32 rbor;
    u32 rbsr;

    ctrl = *(volatile u32 *)0x1000E000;
    pcr = *(volatile u32 *)0x1000E020;
    sqwc = *(volatile u32 *)0x1000E030;
    rbor = *(volatile u32 *)0x1000E050;
    rbsr = *(volatile u32 *)0x1000E040;
    if (env->sts >= 10) {
        return -1;
    }
    if (env->std >= 10) {
        return -2;
    }
    if (env->mfd >= 10) {
        return -3;
    }
    if (env->rcyc >= 7) {
        return -4;
    }
    ctrl = (ctrl & ~0x30) | (dma_sts_control_encodings[env->sts] << 4);
    ctrl = (ctrl & ~0xC0) | (dma_std_control_encodings[env->std] << 6);
    ctrl = (ctrl & ~0xC) | (dma_mfd_control_encodings[env->mfd] << 2);
    if (env->rcyc != 0) {
        ctrl |= 2;
        ctrl = (ctrl & ~0x300) | ((env->rcyc - 1) << 8);
    } else {
        ctrl &= ~2;
    }
    pcr = (env->express << 16) | env->notify;
    sqwc = (env->tqwc << 16) | env->sqwc;
    rbor = (u32)env->rbadr;
    rbsr = env->rbmsk;
    *(volatile u32 *)0x1000E000 = ctrl;
    *(volatile u32 *)0x1000E020 = pcr;
    *(volatile u32 *)0x1000E030 = sqwc;
    *(volatile u32 *)0x1000E050 = rbor;
    *(volatile u32 *)0x1000E040 = rbsr;
    dma_last_applied_environment = *env;
    return 0;
}
