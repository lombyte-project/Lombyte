#include "types.h"

extern u8  gs_draw_env_dma_timeout_message[] __asm__("D_00153078");
extern s32 scePrintf();

s32 sceGsPutDrawEnv(u64 *env) {
    u32 count;
    u32 tag;
    u32 addr;

    count = 0;
    while (*(volatile u32 *)0x1000A000 & 0x100) {
        if (count++ > 0x1000000) {
            scePrintf(gs_draw_env_dma_timeout_message);
            return -1;
        }
    }
    tag = (u32)(*env & 0x7FFF);
    *(volatile u32 *)0x1000A020 = (s32)tag + 1;
    addr = (u32)env;
    if ((addr & 0x70000000) == 0x70000000) {
        *(volatile u32 *)0x1000A010 = (addr & 0x0FFFFFFF) | 0x80000000;
    } else {
        *(volatile u32 *)0x1000A010 = addr & 0x0FFFFFFF;
    }
    *(volatile u32 *)0x1000A000 = 0x101;
    return 0;
}
