#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_002223f0/FUN_002223f0.s", FUN_002223f0);
#else
#include "rnc/fun_002223f0_types.h"
#include "types.h"








extern struct M2c_D_0013C940 D_0013C940;
extern s32 D_001D2AF4[];
extern u8 D_001D5098[];
extern u8 D_001D5148[];
extern u8 D_001D5208[];
extern u8 D_001D5298[];
extern u8 D_001D52B0[];
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern s32 func_001F96F8();
s32 FUN_002223f0(struct M2c_arg0 *arg0) {
    s32 *var_3_180;
    s32 temp_2_44;
    s32 temp_3_100;
    s32 temp_3_53;
    s32 var_2_93;
    s64 temp_7_18;
    u32 temp_2_65;
    u32 var_2_182;

    temp_7_18 = (s64) (((D_0013C940.unk1A0 & 0xF) == 0xF) << 0x20) >> 0x20;
    if ((D_001D5BF0.unkDC == 0) || ((temp_7_18 != 0) && ((s32) D_0013C940.unk1A0 & 0x10))) {
        if ((D_0013C940.unk1C4 & 0xD00) && (D_001D5BF0.unk124 == 0)) {
            return 1;
        }
        if (D_0013C940.unk1C4 & 0x10) {
            temp_2_44 = D_001D5BF0.unk4->unk38;
            if (temp_2_44 != 0) {
                D_001D5BF0.unk8 = temp_2_44;
                goto block_12;
            }
            if (D_001D5BF0.unk124 == 0) {
                return -1;
            }
            goto block_12;
        }
        goto block_12;
    }
block_12:
    temp_3_53 = arg0->unk40;
    arg0->unk40 = (s32) (temp_3_53 - 1);
    if (temp_7_18 != 0) {
        arg0->unk40 = (s32) (temp_3_53 - 2);
    }
    if (arg0->unk40 < 0) {
        arg0->unk40 = 0;
    }
    temp_2_65 = arg0->unk50;
    switch (temp_2_65) {
    case 0:
        arg0->unk50 = (u32) (temp_2_65 + 1);
        arg0->unk40 = func_001F96F8(0xB4, temp_2_65, &D_0013C940, temp_7_18, &D_0013C940, &D_001D5BF0);
        arg0->unk3C = 0;
        D_001D2AF4[0] = 0x50A9;
        arg0->unk34 = D_001D5098;
    default:
block_45:
        var_2_93 = 0;
        break;
    case 1:
    case 5:
    case 8:
    case 12:
    case 16:
        var_2_93 = 0;
        if (arg0->unk40 == 0) {
            arg0->unk54 = 0;
            arg0->unk50 = (u32) (arg0->unk50 + 1);
            goto block_45;
        }
        break;
    case 2:
    case 6:
    case 9:
    case 13:
    case 17:
        temp_3_100 = arg0->unk3C;
        arg0->unk3C = (s32) (temp_3_100 + 0xA);
        if (temp_7_18 != 0) {
            arg0->unk3C = (s32) (temp_3_100 + 0x14);
        }
        var_2_93 = 0;
        if (arg0->unk54 != 0) {
            arg0->unk40 = func_001F96F8(0xB4, temp_2_65, &D_0013C940, temp_7_18, &D_0013C940, &D_001D5BF0);
            arg0->unk50 = (u32) (arg0->unk50 + 1);
            goto block_45;
        }
        break;
    case 4:
        var_2_93 = 0;
        if (arg0->unk40 == 0) {
            arg0->unk50 = 5U;
            arg0->unk40 = func_001F96F8(0xB4, temp_2_65, &D_0013C940, temp_7_18, &D_0013C940, &D_001D5BF0);
            arg0->unk3C = 0;
            D_001D2AF4[0] = 0x50D6;
            arg0->unk34 = D_001D5148;
            goto block_45;
        }
        break;
    case 3:
        var_2_93 = 0;
        if (arg0->unk40 == 0) {
            D_001D2AF4[0] = 0x50D4;
            arg0->unk34 = (void *)0x50D5;
block_37:
            arg0->unk40 = func_001F96F8(0xF0, temp_2_65, &D_0013C940, temp_7_18, &D_0013C940, &D_001D5BF0);
            arg0->unk3C = 0;
            arg0->unk50 = (u32) (arg0->unk50 + 1);
            goto block_45;
        }
        break;
    case 7:
        var_2_93 = 0;
        if (arg0->unk40 == 0) {
            D_001D2AF4[0] = 0x5106;
            arg0->unk34 = D_001D5208;
            goto block_37;
        }
        break;
    case 10:
        var_2_93 = 0;
        if (arg0->unk40 == 0) {
            D_001D2AF4[0] = 0x5136;
            arg0->unk34 = (void *)0x5137;
            goto block_37;
        }
        break;
    case 11:
        if (arg0->unk40 == 0) {
            arg0->unk40 = func_001F96F8(0xB4, temp_2_65, &D_0013C940, temp_7_18, &D_0013C940, &D_001D5BF0);
            arg0->unk3C = 0;
            D_001D2AF4[0] = 0x5138;
            var_3_180 = D_001D5298;
            var_2_182 = 0xC;
block_40:
            arg0->unk50 = var_2_182;
            arg0->unk34 = var_3_180;
        }
        goto block_45;
    case 14:
        var_2_93 = 0;
        if (arg0->unk40 == 0) {
            D_001D2AF4[0] = 0;
            arg0->unk34 = (void *)0x5143;
            goto block_37;
        }
        break;
    case 15:
        var_2_93 = 0;
        if (arg0->unk40 == 0) {
            arg0->unk40 = func_001F96F8(0xB4, temp_2_65, &D_0013C940, temp_7_18, &D_0013C940, &D_001D5BF0);
            arg0->unk3C = 0;
            D_001D2AF4[0] = 0x5144;
            var_3_180 = D_001D52B0;
            var_2_182 = 0x10;
            goto block_40;
        }
        break;
    case 18:
        var_2_93 = 0;
        if (arg0->unk40 == 0) {
            D_001D2AF4[0] = 0;
            arg0->unk34 = (void *)0x5175;
            arg0->unk50 = (u32) (arg0->unk50 + 1);
            arg0->unk40 = func_001F96F8(0x12C, temp_2_65, &D_0013C940, temp_7_18, &D_0013C940, &D_001D5BF0);
            goto block_45;
        }
        break;
    case 19:
        var_2_93 = 0;
        if (arg0->unk40 == 0) {
            var_2_93 = 1;
            if (*(s32 *)((u8 *)(((u8 *)&D_001D5BF0 + 0x5BF0)) + 0xDC) == 0) {
                goto block_45;
            }
        }
        break;
    }
    return var_2_93;
}
#endif /* NON_MATCHING */
