#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022f5b0/FUN_0022f5b0.s", FUN_0022f5b0);
#else
#include "rnc/fun_0022f5b0_types.h"
#include "types.h"


extern struct M2c_D_0013E030 D_0013E030;
extern f32 D_0015ED6C;
extern u8 D_001D9B60[];
extern s32 func_001F96F8();
extern s32 func_001F9A10();
extern s32 func_001F9CF8();
extern s32 func_00213260();
extern s32 func_002132A8(f32, f32);
extern s32 func_00218888();
extern void sp10();
/* retail small-data globals, declared to GAS before the body */
__asm__(".extern D_0015ED6C, 4");

void FUN_0022f5b0(s32 arg0, f32 fparg0) {
    f32 sp10;
    f32 sp14;
    f32 sp18;
    f32 sp1C;
    s64 sp20;
    f32 sp2C;
    s64 sp30;
    f32 temp_f0_43;
    f32 temp_f13_23;
    s32 temp_16_84;
    s32 temp_17_81;
    s32 temp_18_78;
    s32 var_20_14;

    var_20_14 = 0;
    temp_f13_23 = D_0015ED6C;
    do {
        sp10 = func_002132A8(-temp_f13_23, temp_f13_23);
        sp14 = func_002132A8(-D_0015ED6C, D_0015ED6C);
        temp_f0_43 = fparg0 + func_002132A8(D_0015ED6C * -0.25f, D_0015ED6C * 0.25f);
        sp1C = 0.4f;
        sp18 = temp_f0_43;
        sp20 = sp10;
        sp2C = 0.6f;
        sp30 = *((D_0013E030.unk26 * 0x60) + (var_20_14 * 0x10) + D_001D9B60);
        var_20_14 += 1;
        func_001F9CF8(&sp30, &sp30, arg0 + 0xC0);
        func_001F9A10(&sp30, &sp30, arg0 + 0x10);
        temp_18_78 = func_001F96F8(4);
        temp_17_81 = func_001F96F8(4);
        temp_16_84 = func_001F96F8(4);
        func_00218888(&sp30, &sp10, &sp20, 0x24C0C0C0, 0x14C0C0C0, temp_18_78, temp_17_81, temp_16_84 + func_00213260(func_001F96F8(4)), -1);
    } while (var_20_14 < 6);
}
#endif /* NON_MATCHING */
