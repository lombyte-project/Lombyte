#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00214e58/FUN_00214e58.s", FUN_00214e58);
#else
#include "rnc/fun_00214e58_types.h"
#include "types.h"


extern void func_001F9A28();
extern void func_001F9A40();
extern s32 func_001F9B20();
extern s32 func_001F9E90();
extern s32 func_001FA580();
extern s32 func_001FA5C8();
extern s32 func_001FA6D0();
extern void sp30();
void FUN_00214e58(s32 *arg0, s32 arg1, s32 arg2, struct M2c_arg3 *arg3, s32 arg4, f32 fparg0) {    s32 sp1C;
u8 sp_slot[0x100];    s32 sp34;
    s32 sp38;
    s32 spC;
    s64 sp10;
    s64 sp20;
    f32 sp30;
    f32 temp_f20_99;
    f32 temp_f22_95;
    f32 var_f0_97;
    f32 var_f21_101;
    f32 var_f23_102;
    f32 var_f24_41;
    f32 var_f25_66;
    s32 temp_2_30;
    s32 temp_4_50;
    s32 var_17_51;
    s32 var_21_19;
    s32 var_5_54;
    s32 var_6_28;

    var_21_19 = 0;
    var_6_28 = func_001FA6D0();
    if (arg1 == 0) {
        temp_2_30 = *arg0 - 2;
        if (var_6_28 >= temp_2_30) {
            var_6_28 = temp_2_30;
            var_21_19 = 1;
        }
    }
    var_f24_41 = fparg0 - (f32) var_6_28;
    if ((var_21_19 != 0) && (var_f24_41 > 1.0f)) {
        var_f24_41 = 1.0f;
    }
    temp_4_50 = *arg0;
    var_17_51 = var_6_28 + 2;
    var_5_54 = var_6_28 + 1;
    if (var_17_51 >= temp_4_50) {
        var_5_54 = var_5_54 % temp_4_50;
        var_17_51 = var_17_51 % temp_4_50;
    }
    var_f25_66 = 0.0f;
    sp10 = *(s32 *)((u8 *)((var_5_54 * 0x10) + arg0) + 0x10);
    func_001F9A40(arg2, sp_slot, &sp10, var_f24_41, *(s32 *)((u8 *)((var_6_28 * 0x10) + arg0) + 0x10));
    if (!(arg4 & 1)) {
        func_001F9A28(&sp30, &sp10, sp_slot);
        temp_f22_95 = func_001F9E90(sp30, sp34);
        var_f0_97 = func_001F9E90(func_001F9B20(&sp30), sp38);
        temp_f20_99 = var_f0_97;
        var_f21_101 = spC;
        if (var_21_19 != 0) {
            var_f23_102 = temp_f22_95;
            var_f21_101 = 0.0f;
        } else {
            sp20 = *(s32 *)((u8 *)((var_17_51 * 0x10) + arg0) + 0x10);
            func_001F9A28(&sp30, &sp20, &sp10);
            var_f23_102 = func_001F9E90(sp30, sp34);
            var_f0_97 = func_001F9E90(func_001F9B20(&sp30), sp38);
            var_f25_66 = sp1C;
        }
        arg3->unk0 = 0;
        arg3->unk4 = (f32) -func_001FA580(func_001FA5C8(var_f0_97, temp_f20_99) * var_f24_41, temp_f20_99);
        arg3->unk8 = func_001FA580(func_001FA5C8(var_f23_102, temp_f22_95) * var_f24_41, temp_f22_95);
        arg3->unkC = (f32) -func_001FA580(func_001FA5C8(var_f25_66, var_f21_101) * var_f24_41, var_f21_101);
    }
}
#endif /* NON_MATCHING */
