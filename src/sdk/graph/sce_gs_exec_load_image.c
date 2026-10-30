#include "types.h"

extern u8  gs_load_image_dma_timeout_message[] __asm__("D_001531D0");
extern s32 scePrintf();

s32 sceGsExecLoadImage(u8 *lp, u8 *image_payload) {
    u32 count;

    count = 0;
    while (*(volatile u32 *)0x1000A000 & 0x100) {
        if (count++ > 0x1000000) {
            scePrintf(gs_load_image_dma_timeout_message);
            return -1;
        }
    }
    *(volatile u32 *)0x1000A020 = 6;
    if (((u32)lp & 0x70000000) == 0x70000000) {
        *(volatile u32 *)0x1000A010 = ((u32)lp & 0x0FFFFFFF) | 0x80000000;
    } else {
        *(volatile u32 *)0x1000A010 = (u32)lp & 0x0FFFFFFF;
    }
    *(volatile u32 *)0x1000A000 = 0x101;
    while (*(volatile u32 *)0x1000A000 & 0x100) {
        if (count++ > 0x1000000) {
            scePrintf(gs_load_image_dma_timeout_message);
            return -1;
        }
    }
    *(volatile u32 *)0x1000A020 = (s32)(*(u64 *)(lp + 0x50) & 0x7FFF);
    if (((u32)image_payload & 0x70000000) == 0x70000000) {
        *(volatile u32 *)0x1000A010 = ((u32)image_payload & 0x0FFFFFFF) | 0x80000000;
    } else {
        *(volatile u32 *)0x1000A010 = (u32)image_payload & 0x0FFFFFFF;
    }
    *(volatile u32 *)0x1000A000 = 0x101;
    return 0;
}
