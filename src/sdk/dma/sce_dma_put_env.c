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

extern u8        dma_sts_control_encodings[] __asm__("D_00153420");
extern u8        dma_std_control_encodings[] __asm__("D_00153430");
extern u8        dma_mfd_control_encodings[] __asm__("D_00153440");
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
