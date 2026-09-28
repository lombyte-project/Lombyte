#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00239f58/FUN_00239f58.s", FUN_00239f58);
#else
#include "rnc/fun_00239f58_types.h"
#include "types.h"




extern s32 D_001610E0;
extern s32 D_00161190;
extern struct M2c_D_001E66E0 D_001E66E0;
extern s32 func_001F9AD8();
extern s32 func_001F9BF8();
extern s32 func_001FA6C0();
extern s32 func_001FA6D0();
extern s32 func_00239D60();
/* retail small-data globals, declared to GAS before the body */
__asm__(".extern D_001610E0, 4");

s32 FUN_00239f58(f32 *arg0, s32 arg1, f32 fparg0, f32 fparg1) {
u8 sp_slot[0xC0];    s32 sp10;
    f32 sp14;
    f32 sp18;
    f32 sp1C;
    f32 temp_f13_68;
    f32 temp_f1_155;
    f32 temp_f20_38;
    f32 temp_f20_54;
    f32 temp_f21_39;
    f32 temp_f4_70;
    f32 var_f0_116;
    f32 var_f3_95;
    f32 var_f5_75;
    s32 temp_18_43;
    s32 temp_19_46;
    s32 temp_2_19;
    s32 temp_4_61;
    s32 temp_5_58;
    s32 var_2_23;
    struct M2c_temp_20_32 *temp_20_32;
    s32 *temp_6_63;

    temp_2_19 = func_00239D60();
    var_2_23 = 0;
    if (temp_2_19 >= 0) {
        temp_20_32 = D_00161190 + (temp_2_19 * 0x1190);
        temp_f20_38 = fparg0 - (temp_20_32->unk0 + D_001E66E0.unk8);
        temp_f21_39 = fparg1 - (temp_20_32->unk4 + D_001E66E0.unkC);
        temp_18_43 = func_001FA6D0(temp_2_19, temp_f20_38 / D_001E66E0.unk10);
        temp_19_46 = func_001FA6D0((s32)(u32) (temp_f21_39 / D_001E66E0.unk14));
        temp_f20_54 = (temp_f20_38 - (func_001FA6C0(temp_18_43) * D_001E66E0.unk10)) / D_001E66E0.unk10;
        temp_5_58 = temp_19_46 << 6;
        temp_4_61 = D_001610E0 * 0x5C0;
        temp_6_63 = ((u8 *)temp_20_32) + 0x50;
        temp_f13_68 = (temp_f21_39 - (func_001FA6C0(temp_19_46) * D_001E66E0.unk14)) / D_001E66E0.unk14;
        temp_f4_70 = *(temp_6_63 + ((temp_18_43 * 4) + temp_5_58 + temp_4_61));
        if (temp_18_43 == 0xF) {
            var_f5_75 = *(s32 *)((u8 *)(((u8 *)temp_20_32) + ((temp_19_46 * 4) + temp_4_61)) + 0x510);
        } else {
            var_f5_75 = *(temp_6_63 + (((temp_18_43 + 1) * 4) + temp_5_58 + temp_4_61));
        }
        if (temp_19_46 == 0xF) {
            var_f3_95 = *(s32 *)((u8 *)(((u8 *)temp_20_32) + ((temp_18_43 * 4) + (D_001610E0 * 0x5C0))) + 0x4D0);
        } else {
            var_f3_95 = *(s32 *)((u8 *)(((u8 *)temp_20_32) + ((temp_18_43 * 4) + ((temp_19_46 + 1) << 6) + (D_001610E0 * 0x5C0))) + 0x50);
        }
        if (temp_18_43 == 0xF) {
            if (temp_19_46 == temp_18_43) {
                var_f0_116 = *(s32 *)((u8 *)(((u8 *)temp_20_32 + ((D_001610E0 * 0x5C0)))) + 0x5DC);
            } else {
                var_f0_116 = *(s32 *)((u8 *)(((u8 *)temp_20_32) + (((temp_19_46 + 1) * 4) + (D_001610E0 * 0x5C0))) + 0x510);
            }
        } else if (temp_19_46 == 0xF) {
            var_f0_116 = *(s32 *)((u8 *)(((u8 *)temp_20_32) + (((temp_18_43 + 1) * 4) + (D_001610E0 * 0x5C0))) + 0x4D0);
        } else {
            var_f0_116 = *(s32 *)((u8 *)(((u8 *)temp_20_32) + (((temp_18_43 + 1) * 4) + ((temp_19_46 + 1) << 6) + (D_001610E0 * 0x5C0))) + 0x50);
        }
        if (arg0 != NULL) {
            temp_f1_155 = temp_f4_70 + ((var_f5_75 - temp_f4_70) * temp_f20_54);
            *arg0 = temp_f1_155 + (((var_f3_95 + ((var_f0_116 - var_f3_95) * temp_f20_54)) - temp_f1_155) * temp_f13_68) + temp_20_32->unk8;
        }
        if (arg1 != 0) {
            sp14 = D_001E66E0.unk14;
            sp18 = var_f3_95 - temp_f4_70;
            sp10 = 0;
            sp1C = 1.0f;
            func_001F9AD8(arg1, sp_slot, &sp10, D_001E66E0.unk10, 0, var_f5_75 - temp_f4_70, 1.0f);
            func_001F9BF8(arg1, arg1, 0x3F800000);
        }
        var_2_23 = 1;
    }
    return var_2_23;
}

extern s32 func_00239F58(f32 *arg0, s32 arg1, f32 fparg0, f32 fparg1) __attribute__((alias("FUN_00239f58")));
#endif /* NON_MATCHING */
