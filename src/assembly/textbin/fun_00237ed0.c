#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00237ed0/FUN_00237ed0.s", FUN_00237ed0);
#else
#include "rnc/fun_00237ed0_types.h"
#include "types.h"






extern u8 D_001516EC[];
extern s16 D_0015172A;
extern u8 D_0015EDB0;
extern u8 D_00161030[];
extern u8 D_00161038[];
extern u8 D_00161040[];
extern s32 D_001610B0;
extern s32 D_001610B4;
extern struct M2c_D_001E63C0 D_001E63C0;
extern s32 D_001E63FC[];
extern struct M2c_D_001E65E0 D_001E65E0;
extern s32 func_001F96F8();
extern s32 func_0020CB10();
extern s32 func_0020CB88();
extern s32 func_00212F90();
extern s32 func_00213260();
extern s32 func_00214CC8();
extern s32 func_00215B10();
void FUN_00237ed0(struct M2c_arg0 *arg0) {
    s32 *var_6_138;
    s32 temp_16_209;
    s32 temp_16_240;
    s32 temp_16_271;
    s32 temp_2_58;
    s32 temp_3_66;
    u8 temp_3_38;
    u8 temp_5_205;
    u8 var_2_211;

    if (D_0015EDB0 != 0) {
        if (D_001E65E0.unk1 == 0) {
            func_0020CB10(0, &D_001E65E0);
        }
        D_001E65E0.unk28 = 2.37f;
        D_001E65E0.unk20 = 2.37f;
        D_001E65E0.unk24 = 2.37f;
    } else if (D_001E65E0.unk1 != 0) {
        func_0020CB88(arg0, &D_001E65E0);
    }
    temp_3_38 = arg0->unk20;
    switch (temp_3_38) {
    case 0:
        if (func_001F96F8(0x258) < D_001E63C0.unk34) {
            temp_2_58 = func_00213260(2);
            temp_3_66 = ((temp_2_58 * 3) + 1) * 6;
            D_001610B0 = 1;
            D_001E63C0.unk3C = temp_2_58;
            D_001610B4 = 0;
            *(s32 *)D_001516EC = temp_3_66 + D_001E63C0.unk38 + 0x2710;
            if (arg0->unk53 != 2) {
                func_00212F90(arg0, 2, 0, func_001F96F8(0x12, 6U, D_001E63C0.unk38, D_001516EC));
            }
            arg0->unk20 = 0xCU;
block_16:
            D_001E63C0.unk34 = 0;
        }
block_17:
        D_001E63C0.unk34 = (s32) (D_001E63C0.unk34 + 1);
    default:
        return;
    case 2:
        if (func_001F96F8(0x258) < D_001E63C0.unk34) {
            if (arg0->unk53 != 3) {
                func_00212F90(arg0, 3, 0, func_001F96F8(0x12));
            }
            arg0->unk20 = 3U;
            goto block_16;
        }
        goto block_17;
    case 3:
        if (arg0->unk70 & 2) {
            if (arg0->unk53 == 0) {
                arg0->unk20 = 0U;
                return;
            }
            func_00212F90(arg0, 0, 0, func_001F96F8(0x12));
            arg0->unk20 = 0U;
        }
        break;
    case 4:
        var_6_138 = D_00161030;
block_26:
        if ((func_00214CC8(arg0, &D_001E63C0, var_6_138, (f32) *((D_001E63C0.unk3C * 4) + var_6_138)) != 0) && (D_001610B4 != 0)) {
            D_001610B4 = 0;
            func_00215B10();
        }
        if (arg0->unk70 & 2) {
            if (arg0->unk53 != 2) {
                func_00212F90(arg0, 2, 0, func_001F96F8(0x12));
            }
            arg0->unk20 = 2U;
            D_001E63C0.unk34 = 0;
        }
        break;
    case 5:
        var_6_138 = D_00161038;
        goto block_26;
    case 6:
        var_6_138 = D_00161040;
        goto block_26;
    case 10:
        if ((D_0015172A == 3) && (D_001610B0 != 0)) {
            D_001610B0 = 0;
            D_001610B4 = 1;
        }
        if (arg0->unk70 & 2) {
            temp_5_205 = arg0->unk53;
            temp_16_209 = (D_001E63FC[0] * 3) + 4;
            if (temp_5_205 == temp_16_209) {
                var_2_211 = 4;
            } else {
                func_00212F90(arg0, temp_16_209, 0, func_001F96F8(0xC, temp_5_205));
                var_2_211 = 4;
            }
block_51:
            arg0->unk20 = var_2_211;
        }
        break;
    case 12:
        if ((D_0015172A == 3) && (D_001610B0 != 0)) {
            D_001610B4 = 1;
            D_001610B0 = 0;
            temp_16_240 = (D_001E63FC[0] * 3) + 5;
            if (arg0->unk53 == temp_16_240) {
                var_2_211 = 5;
            } else {
                func_00212F90(arg0, temp_16_240, 0, func_001F96F8(0x12));
                var_2_211 = 5;
            }
            goto block_51;
        }
        break;
    case 11:
        if ((D_0015172A == 3) && (D_001610B0 != 0)) {
            D_001610B4 = 1;
            D_001610B0 = 0;
            temp_16_271 = (D_001E63FC[0] * 3) + 6;
            var_2_211 = 6;
            if (arg0->unk53 != temp_16_271) {
                func_00212F90(arg0, temp_16_271, 0, func_001F96F8(0x12));
                var_2_211 = 6;
            }
            goto block_51;
        }
        break;
    }
}
#endif /* NON_MATCHING */
