#include "types.h"
struct M2c_D_0019A3E8 {
    u8 pad_0[0x10];
    s32 unk10;
    s32 unk14;
};

extern struct M2c_D_0019A3E8 D_0019A3E8;
extern s32 InitializeResourceEntry();
s32 hud_heap_alloc(s32 arg0) __asm__("FUN_001ff288");

s32 hud_heap_alloc(s32 arg0) {
    register s32 arg_c __asm__("s0");
    register s32 var_2_20 __asm__("v0");
    register s32 next __asm__("a0");
    register s32 tmp __asm__("a1");
    register s32 size __asm__("s0");

    arg_c = arg0;
    if (D_0019A3E8.unk10 == 0) {
        InitializeResourceEntry();
    }
    if ((D_0019A3E8.unk14 - D_0019A3E8.unk10) >= arg_c) {
        goto allocate;
    }
    return 0;
allocate:
        var_2_20 = D_0019A3E8.unk10;
        tmp = arg_c + 0xF;
        size = tmp & 0xFFFFFFF0;
        __asm__ volatile ("" : "+r" (var_2_20));
        next = D_0019A3E8.unk10;
        next = next + size;
        D_0019A3E8.unk10 = next;
        return var_2_20;
}

extern __typeof__(hud_heap_alloc) func_001ff288 __attribute__((alias("FUN_001ff288")));
