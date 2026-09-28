#include "types.h"
struct M2c_D_001E63C0 {
    u8 pad_0[0x4];
    s32 unk4;
    u8 pad_8[0x44];
    s32 unk4C;
    u8 pad_50[0x8];
    s32 unk58;
    u8 pad_5C[0x1B4];
    s32 unk210;
};

struct M2c_1863D0_ent {
    u8 pad_0[0x38];
    u16 unk38;
};

extern u8 D_001863D0[];
extern struct M2c_D_001E63C0 D_001E63C0;
extern s32 SubtractIntegerWithClamp();
extern void func_001FB8F0();
extern s32 func_001FF960();
extern void func_001FFC30();
void FUN_002386e8(void) {
    s32 temp_16_35;
    s32 temp_3_162;
    s32 temp_58_3;
    s32 var_16_144;
    s32 var_17_145;
    s32 var_18_33;
    s32 var_19_31;
    s32 var_2_171;
    s32 var_2_74;
    u16 temp_4_170;
    u16 temp_4_73;
    s32 *var_16_65;
    s32 arg7;

    func_001FB8F0(0, 0, 0x200, 0x80, 0x200, 0x80, 0);
    if (D_001E63C0.unk210 < 8) {
        var_19_31 = 0xC;
        var_18_33 = 0;
        temp_16_35 = D_001E63C0.unk58 * 0x38;
        arg7 = ((SubtractIntegerWithClamp(((D_001E63C0.unk4 * 4) & 0x3F) - 0x20) + 0x40) * 0x10202) | 0x80000000;
        func_001FB8F0(temp_16_35 + 8, 2, temp_16_35 + 0x40, 0x3A, 0x200, 0x80, arg7);
        func_001FB8F0(temp_16_35 + 0xA, 4, temp_16_35 + 0x3E, 0x38, 0x200, 0x80, 0x80000000);
        if (D_001E63C0.unk210 > 0) {
            do {
                var_16_65 = (s32 *) (void *) ((u8 *)&D_001E63C0 + 0xD4 + var_18_33 * 0x14);
                temp_4_73 = ((struct M2c_1863D0_ent *) (void *) ((u8 *)D_001863D0 + var_16_65[-1] * 0x4C))->unk38;
                if (var_16_65[0] == 1) {
                    var_2_74 = func_001FF960(temp_4_73, 2);
                } else {
                    var_2_74 = func_001FF960(temp_4_73, 0);
                }
                func_001FFC30(var_2_74, var_19_31, 6, 0x30, 0x30, 0x80);
                var_19_31 += 0x38;
                var_18_33 += 1;
            } while (var_18_33 < D_001E63C0.unk210);
        }
    } else {
        if (D_001E63C0.unk4C < 0) {
            D_001E63C0.unk4C = (s32) (D_001E63C0.unk4C + 4);
        } else if (D_001E63C0.unk4C > 0) {
            D_001E63C0.unk4C = (s32) (D_001E63C0.unk4C - 4);
        } else {
            func_001FB8F0(0xB0, 2, 0xE8, 0x3A, 0x200, 0x80, ((SubtractIntegerWithClamp(((D_001E63C0.unk4 * 4) & 0x3F) - 0x20) + 0x40) * 0x10202) - (s32) 0x80000000);
            func_001FB8F0(0xB2, 4, 0xE6, 0x38, 0x200, 0x80, 0x80000000);
        }
        var_16_144 = D_001E63C0.unk4C - 0x64;
        var_17_145 = -2;
        do {
            temp_58_3 = D_001E63C0.unk58 - 3;
            temp_3_162 = ((s32) ((D_001E63C0.unk210 * 2) + var_17_145 + temp_58_3) % (s32) D_001E63C0.unk210) * 0x14;
            temp_4_170 = ((struct M2c_1863D0_ent *) (void *) ((u8 *)D_001863D0 + *(s32 *)(void *)(temp_3_162 + ((u8 *)&D_001E63C0 + 0xD0)) * 0x4C))->unk38;
            if (*(s32 *)(void *)(temp_3_162 + ((u8 *)&D_001E63C0 + 0xD4)) == 1) {
                var_2_171 = func_001FF960(temp_4_170, 2);
            } else {
                var_2_171 = func_001FF960(temp_4_170, 0);
            }
            var_17_145 += 1;
            func_001FFC30(var_2_171, var_16_144, 6, 0x30, 0x30, 0x80808080);
            var_16_144 += 0x38;
        } while (var_17_145 < 9);
    }
}
