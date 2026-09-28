#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/text/font_print_window/FUN_001f7090.s", FUN_001f7090);
#else
#include "rnc/ui_text_font_print_window_types.h"
#include "types.h"






extern struct M2c_D_0013E500 D_0013E500;
extern s32 D_0015F49C;
extern s32 D_0015F4A0;
extern u8 D_0018CAF8[];
extern s32 func_001F6200();
extern void func_001F62B0();
extern void func_001F6638();
extern void func_00233A40();
/* retail small-data globals, declared to GAS before the body */
__asm__(".extern D_0015F4A0, 4");

void font_print_window(struct M2c_arg0 *arg0, s64 arg1, u8 *arg2, s32 arg3, s64 arg4, s32 arg5) __asm__("FUN_001f7090");

void font_print_window(struct M2c_arg0 *arg0, s64 arg1, u8 *arg2, s32 arg3, s64 arg4, s32 arg5) {
u8 sp_slot[0x190];    s32 sp40;
    s32 sp80;
    s64 spC0;
    s64 spC8;
    s32 spD0;
    s32 *spD4;
    f32 var_f0_235;
    f32 var_f1_239;
    s16 *temp_6_115;
    s16 temp_2_207;
    s16 temp_3_172;
    s16 temp_5_201;
    s16 temp_5_40;
    s16 temp_7_125;
    s16 var_17_175;
    s16 var_18_50;
    s16 var_7_59;
    s16 var_9_74;
    s32 temp_18_206;
    s32 temp_2_228;
    s32 temp_2_43;
    s32 temp_5_44;
    s32 var_17_52;
    s32 var_21_57;
    s32 var_23_188;
    s32 var_25_53;
    s32 var_2_62;
    s32 var_30_182;
    s32 var_30_51;
    s32 var_5_38;
    s32 var_8_76;
    s8 temp_4_108;
    u16 temp_3_225;
    u8 *var_6_80;
    u8 temp_2_127;
    u8 temp_2_83;
    struct M2c_var_19_187 *var_19_187;

    spC0 = arg1;
    spC8 = arg4;
    func_00233A40(arg0->unk4, arg0->unk6 - 1, arg0->unk0, arg0->unk2 - 1);
    D_0015F4A0 = 1;
    if ((arg0->unk12 ^ 1) & 1) {
        var_5_38 = arg0->unk6 - arg0->unk8;
    } else {
        temp_5_40 = arg0->unk8;
        temp_2_43 = arg0->unk6 - temp_5_40;
        temp_5_44 = temp_5_40 - arg0->unk4;
        var_5_38 = ((temp_2_43 < temp_5_44) ? temp_2_43 : temp_5_44) * 2;
    }
    spD0 = var_5_38;
    var_18_50 = 0;
    var_30_51 = 0;
    var_17_52 = 0;
    var_25_53 = 0;
loop_4:
    var_21_57 = 0;
    var_7_59 = 0;
    if (arg3 != 0) {
        var_2_62 = 0 * 2;
        if (*arg2 != 0) {
loop_7:
            *(sp_slot + var_2_62) = var_7_59;
            *(&sp80 + var_2_62) = var_18_50;
            var_9_74 = var_7_59;
            var_8_76 = 0;
            if (var_5_38 > 0) {
                var_6_80 = var_7_59 + arg2;
loop_9:
                temp_2_83 = *var_6_80;
                if ((temp_2_83 == 0x20) || (temp_2_83 < 0x10U)) {
                    var_9_74 = var_7_59;
                }
                if (D_0015F49C != 0) {
                    if ((u32) (*var_6_80 - 8) < 8U) {
                        var_18_50 = *var_6_80 - 8;
                    }
                }
                if (*var_6_80 >= 2U) {
                    var_7_59 += 1;
                    temp_4_108 = *(s32 *)((u8 *)((*var_6_80 * 4) + arg5) + 0x3);
                    var_8_76 = (temp_4_108 != 0) ? (var_8_76 + temp_4_108) : var_8_76;
                    var_6_80 += 1;
                    if (var_8_76 < var_5_38) {
                        goto loop_9;
                    }
                }
            }
            temp_6_115 = &sp40 + var_2_62;
            *temp_6_115 = var_9_74;
            if (var_9_74 == *(sp_slot + var_2_62)) {
                *temp_6_115 = var_7_59;
            }
            temp_7_125 = *temp_6_115;
            temp_2_127 = *(arg2 + temp_7_125);
            if ((temp_2_127 == 0x20) || (temp_2_127 < 0x10U)) {
                *temp_6_115 = (u16) *temp_6_115 - 1;
            }
            var_21_57 += 1;
            if (*(arg2 + temp_7_125) != 0) {
                var_7_59 = temp_7_125 + 1;
                if (var_7_59 != arg3) {
                    var_2_62 = var_21_57 * 2;
                    if (*(arg2 + var_7_59) != 0) {
                        goto loop_7;
                    }
                }
            } else {
                var_25_53 = var_8_76;
            }
        }
    }
    if (var_30_51 == 0) {
        var_17_52 = (var_17_52 == 0) ? var_21_57 : var_17_52;
        if (var_21_57 >= 2) {
            if (var_17_52 >= var_21_57) {
                var_5_38 -= 0x10;
                if (var_25_53 < (var_5_38 / 3)) {
                    goto loop_4;
                }
            } else {
                var_5_38 = spD0;
                var_30_51 = 1;
                goto loop_4;
            }
        }
    }
    temp_3_172 = var_21_57 * arg0->unk10;
    arg0->unkC = 0;
    var_17_175 = arg0->unkA;
    arg0->unkE = temp_3_172;
    if (arg0->unk12 & 2) {
        var_17_175 -= temp_3_172 >> 1;
    }
    var_30_182 = 0;
    if (var_21_57 > 0) {
        spD4 = &sp80;
        var_19_187 = sp_slot;
        var_23_188 = 0;
        do {
            if (((var_17_175 + arg0->unk10) >= arg0->unk0) && (arg0->unk2 >= var_17_175)) {
                temp_5_201 = var_19_187->unk0;
                temp_18_206 = (*(s32 *)((u8 *)(sp_slot + var_23_188) + 0x40) - temp_5_201) + 1;
                temp_2_207 = func_001F6200(arg2 + temp_5_201, temp_18_206, arg5);
                if (arg0->unkC < temp_2_207) {
                    arg0->unkC = temp_2_207;
                }
                if (!(arg0->unk12 & 4)) {
                    *(s32 *)D_0018CAF8 = (s32) ((s64) (spC0 << 0x20) >> 0x20);
                    temp_3_225 = arg0->unk12;
                    temp_2_228 = temp_3_225 & 1;
                    if (temp_3_225 & 8) {
                        if (temp_2_228 != 0) {
                            var_f0_235 = (f32) arg0->unk14;
    __asm__ volatile ("" : "+f" (var_f0_235));
                            var_f1_239 = (f32) (arg0->unk8 - (temp_2_207 >> 1));
                        } else {
                            var_f0_235 = (f32) arg0->unk14;
                            var_f1_239 = (f32) arg0->unk8;
                        }
                        func_001F6638(((s32 *)D_0018CAF8 + var_19_187->unk80), arg2 + var_19_187->unk0, temp_18_206, spC8, arg5, var_f1_239 + (var_f0_235 * 0.0625f), (f32) var_17_175 + ((f32) arg0->unk16 * 0.0625f), 0x3F800000);
                    } else if (temp_2_228 != 0) {
                        func_001F62B0(arg0->unk8 - (temp_2_207 >> 1), var_17_175, ((s32 *)D_0018CAF8 + *(s32 *)((u8 *)spD4 + var_23_188)), arg2 + var_19_187->unk0, temp_18_206, spC8, arg5);
                    } else {
                        func_001F62B0(arg0->unk8, var_17_175, ((s32 *)D_0018CAF8 + *(s32 *)((u8 *)spD4 + var_23_188)), arg2 + var_19_187->unk0, temp_18_206, spC8, arg5);
                    }
                }
            }
            var_30_182 += 1;
            var_19_187 += 2;
            var_23_188 += 2;
            var_17_175 += arg0->unk10;
        } while (var_30_182 < var_21_57);
    }
    D_0015F4A0 = 0;
    func_00233A40(0, D_0013E500.unk0 - 1, 0, D_0013E500.unk4 - 1);
}

extern void func_001F7090(struct M2c_arg0 *arg0, s64 arg1, u8 *arg2, s32 arg3, s64 arg4, s32 arg5) __attribute__((alias("FUN_001f7090")));
#endif /* NON_MATCHING */
