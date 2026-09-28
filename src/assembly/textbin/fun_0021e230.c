#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021e230/FUN_0021e230.s", FUN_0021e230);
#else
#include "rnc/fun_0021e230_types.h"
#include "types.h"






























extern u8 D_0013E520[];
extern s32 D_00140408;
extern s32 D_0015FF50[];
extern struct M2c_D_001863D0 D_001863D0;
extern struct M2c_D_00186F40 D_00186F40;
extern u8 D_001B3200[];
extern u8 D_001B3AC0[];
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern u8 D_001E0408[];
extern s32 func_001E9470();
extern s32 func_001E9478();
extern s32 func_001FA580();
extern void func_00204A40();
extern s32 func_00212ED8();
extern s32 func_00225490();
extern s32 func_00225530();
extern void func_0021E698();
s32 FUN_0021e230(struct M2c_arg0 *arg0) {
    f32 var_f0_158;
    s16 temp_21_25;
    s16 var_23_27;
    s32 temp_17_133;
    s32 temp_17_32;
    s32 temp_18_35;
    s32 temp_18_62;
    s32 temp_19_44;
    s32 temp_22_58;
    s32 temp_2_47;
    s32 temp_30_50;
    s32 temp_3_59;
    s32 temp_4_22;
    s32 temp_5_200;
    s32 temp_5_235;
    s32 var_22_37;
    s32 var_4_154;
    struct M2c_temp_2_127 *temp_2_127;
    struct M2c_temp_2_169 *temp_2_169;
    struct M2c_temp_2_210 *temp_2_210;
    s32 *temp_2_250;
    struct M2c_temp_3_20 *temp_3_20;
    struct M2c_temp_3_216 *temp_3_216;
    struct M2c_temp_3_228 *temp_3_228;
    struct M2c_temp_7_182 *temp_7_182;
    struct M2c_temp_7_19 *temp_7_19;

    temp_7_19 = arg0->unk44;
    temp_3_20 = D_001D5BF0.unk4->unk40;
    temp_4_22 = temp_3_20->unk48;
    temp_21_25 = *(s32 *)((u8 *)((temp_3_20->unk3C * 0xA) + temp_4_22) + 0x6);
    var_23_27 = -1;
    if (temp_7_19 != NULL) {
        var_23_27 = temp_7_19->unkA6;
    }
    temp_17_32 = temp_21_25 * 0x4C;
    temp_18_35 = temp_21_25 ^ 0x18;
    var_22_37 = 0;
    temp_19_44 = (temp_18_35 == 0) ? 0x1DF : *(s32 *)((u8 *)(((u8 *)&D_001863D0) + temp_17_32) + 0x10);
    arg0->unk38 = func_001FA580(temp_4_22, D_001D5BF0.unk4, 0xA, temp_7_19, arg0->unk38, 0x3C23D70A);
    temp_2_47 = *(s32 *)((u8 *)(((u8 *)&D_001863D0) + temp_17_32) + 0x8);
    temp_30_50 = temp_2_47 == 3;
    if (((temp_2_47 ^ 2) != 0) && (temp_30_50 == 0)) {
        var_22_37 = temp_2_47 != 1;
    }
    temp_22_58 = (temp_18_35 == 0) ? 0 : var_22_37;
    temp_3_59 = arg0->unk34;
    temp_18_62 = arg0->unk30 & 1;
    switch (temp_3_59) {                            /* irregular */
    case 1:
        if (temp_19_44 != -1) {
            arg0->unk34 = 2;
        }
        break;
    case 2:
        if (temp_19_44 == -1) {
        case 0:
            arg0->unk34 = 1;
        } else {
            if (temp_22_58 != 0) {
                if ((D_00140408 != 0) && (temp_19_44 != D_00140408)) {
                    func_001E9470(0, 0);
                }
                if (temp_19_44 != D_001D5BF0.unk11C) {
                    func_00204A40(temp_19_44, D_001D5BF0.unk118 == 0);
                    D_001D5BF0.unk144 = (s32) D_0015FF50[0];
                    D_001D5BF0.unk120 = temp_19_44;
                    D_001D5BF0.unk140 = temp_19_44;
                    *(s32 *)((u8 *)(*((*(s32 *)(temp_19_44 + D_001B3AC0) * 4) + D_001B3200)) + 0xD) = 0;
                }
            }
            temp_2_127 = func_00225490(temp_19_44);
            temp_17_133 = ((temp_21_25 ^ 2) == 0) ? 6 : 1;
            if (temp_2_127 != NULL) {
                if ((temp_22_58 != 0) && (*(s32 *)(temp_21_25 + D_0013E520) != 0)) {
                    func_001E9478(temp_2_127, arg0->unk14);
                }
                arg0->unk44 = temp_2_127;
                temp_2_127->unk34 = 0;
                if (temp_18_62 != 0) {
                    var_4_154 = temp_21_25 << 5;
                    var_f0_158 = *(s32 *)(var_4_154 + D_001E0408);
                } else {
                    var_4_154 = temp_21_25 << 5;
                    var_f0_158 = *(s32 *)((u8 *)(D_001E0408 + var_4_154) + 0x4);
                }
                temp_2_127->unk10 = (f32) (D_00186F40.unk140 + var_f0_158);
                temp_2_169 = D_001E0408 + var_4_154;
                temp_7_182 = temp_2_127->unk78;
                temp_2_127->unk14 = (f32)(u32)(D_00186F40.unk144 + temp_2_169->unk8);
                temp_2_127->unk18 = (f32)(u32)(D_00186F40.unk148 + temp_2_169->unkC);
                temp_2_127->unk40 = (f32)(u32)temp_2_169->unk10;
                temp_2_127->unk74 = &func_0021E698;
                temp_2_127->unk48 = 3.1415927f;
                temp_2_127->unk44 = (f32)(u32)temp_2_169->unk14;
                temp_7_182->unkC = (s32) temp_21_25;
                temp_7_182->unk0 = arg0;
                temp_5_200 = temp_2_127->unk24->unkC - 1;
                func_00212ED8(temp_2_127, (temp_17_133 < temp_5_200) ? temp_17_133 : temp_5_200, 0, temp_7_182, temp_2_169, temp_2_169);
            }
            if ((temp_30_50 != 0) && (temp_2_127 != NULL)) {
                temp_2_210 = func_00225490(D_001863D0.unk5C);
                if (temp_2_210 != NULL) {
                    temp_2_210->unk34 = 0;
                    temp_3_216 = arg0->unk44;
                    temp_2_210->unk10 = (s64) temp_3_216->unk10;
                    temp_2_210->unk40 = (s64) temp_3_216->unk40;
                    arg0->unk48 = temp_2_210;
                    temp_3_228 = temp_2_210->unk78;
                    temp_2_210->unk74 = &func_0021E698;
                    temp_3_228->unkC = (s32) temp_21_25;
                    temp_3_228->unk0 = arg0;
                    temp_5_235 = temp_2_210->unk24->unkC - 1;
                    func_00212ED8(temp_2_210, (temp_17_133 < temp_5_235) ? temp_17_133 : temp_5_235, 0);
                }
            }
            arg0->unk34 = 3;
        }
        break;
    case 3:
        if (var_23_27 != temp_19_44) {
            arg0->unk44 = func_00225530(arg0->unk44);
            temp_2_250 = func_00225530(arg0->unk48);
            arg0->unk34 = 2;
            arg0->unk48 = temp_2_250;
        }
        break;
    }
    return 0;
}

extern s32 func_0021E230(struct M2c_arg0 *arg0) __attribute__((alias("FUN_0021e230")));
#endif /* NON_MATCHING */
