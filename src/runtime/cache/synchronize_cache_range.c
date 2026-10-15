#include "types.h"
#include "kernel.h"

extern s32 _sceSDC();
extern s32 DIntr();
extern s32 EnableInterrupts();

void synchronize_cache_range(s32 arg0, s32 arg1) __asm__("FUN_00118f88");

void synchronize_cache_range(s32 arg0, s32 arg1) {
    u32 status;

    status = CpuReadStatus();
    status &= 0x10000;
    if (status != 0) {
        DIntr();
    }
    _sceSDC(arg0 & 0xFFFFFFC0, arg1 & 0xFFFFFFC0);
    if (status != 0) {
        EnableInterrupts();
    }
}
