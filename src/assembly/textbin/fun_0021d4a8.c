#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021d4a8/FUN_0021d4a8.s", FUN_0021d4a8);
#else
#include "rnc/fun_0021d4a8_types.h"
#include "types.h"




extern u8 D_0013D408[];
extern s32 D_0015ED88;
extern struct M2c_D_001D5BF4 *D_001D5BF4;
extern u8 D_001DF050[];
extern u8 D_001DF3F0[];
extern u8 D_001DF790[];
extern void func_001F4280();
extern void func_001F4398();
extern s32 func_001F44B8();
extern void func_001F61E8();
extern void func_001F61F8();
extern void func_001F7090();
extern s32 func_001FDD10();
extern void func_0021F8E8(s32, s32, s32);
extern void func_00233980(s32, s64);
extern void memset();
s32 FUN_0021d4a8(struct M2c_arg0 *arg0) {    u16 sp2;
u8 sp_slot[0xF0];    u16 spE;
    s16 sp8;
    u16 spA;                                        /* compiler-managed */
    s16 sp12;
    s16 sp20;
    s16 sp22;
    s16 sp26;
    s16 sp2A;
    s16 sp30;
u8 *sp40;
    s32 *sp44;
    s32 var_16_139;
    s32 var_16_145;
    s32 var_19_14;
    s32 var_2_147;
    s16 *var_5_53;
    s16 temp_3_267;
    s16 temp_4_247;
    s16 var_4_180;
    s32 temp_18_177;
    s32 temp_22_115;
    s32 temp_3_176;
    s32 temp_3_304;
    s32 temp_5_21;
    s32 temp_5_279;
    s32 temp_6_254;
    s32 temp_6_266;
    s32 var_18_10;
    s32 var_19_124;
    s32 var_20_120;
    s32 var_21_86;
    s32 var_23_71;
    s32 var_2_288;
    s32 var_3_58;
    u16 temp_2_244;
    u16 temp_2_262;
    u16 var_2_249;

    var_18_10 = 0xC;
    var_19_14 = 1;
    temp_5_21 = arg0->unk30;
    sp44 = D_001DF050;
    sp40 = D_001D5BF4->unk40 == (s32) arg0;
    if (temp_5_21 & 4) {
        var_18_10 = 0xE;
        var_19_14 = 3;
        sp44 = D_001DF790;
    }
    if (temp_5_21 & 8) {
        var_18_10 = 0xA;
        var_19_14 = 2;
        sp44 = D_001DF3F0;
    }
    func_00233980(0x42, 0x44);
    func_00233980(0x47, 0x2004B);
    func_001F4280(0);
    var_5_53 = arg0->unk34;
    var_3_58 = 0;
    if (*var_5_53 != 0) {
        do {
            var_5_53 += 0xC;
            var_3_58 += 1;
        } while (*var_5_53 != 0);
    }
    var_23_71 = var_18_10 + 3;
    if (!(arg0->unk30 & 0x10)) {
        var_23_71 = (s32) arg0->unk24 / (s32) (var_3_58 + 1);
    }
    var_21_86 = 0;
    memset(&sp20, 0, 0x18);
    sp22 = (u16) arg0->unk24 - 4;
    sp20 = 4;
    sp26 = arg0->unk20 - 2;
    sp2A = (var_23_71 - ((s32) (var_18_10 + ((u32) (var_18_10 >> 0x1F) >> 0x1F)) >> 1)) - arg0->unk44;
    sp30 = var_18_10 + 2;
    /* m2c-unknown:  unknown instruction: ldl $v0, 0x27($sp_slot)  */
    /* m2c-unknown:  unknown instruction: ldr $v0, 0x20($sp_slot)  */
    /* m2c-unknown:  unknown instruction: ldl $v1, 0x2f($sp_slot)  */
    /* m2c-unknown:  unknown instruction: ldr $v1, 0x28($sp_slot)  */
    /* m2c-unknown:  unknown instruction: ldl $a1, 0x37($sp_slot)  */
    /* m2c-unknown:  unknown instruction: ldr $a1, 0x30($sp_slot)  */
    /* m2c-unknown:  unknown instruction: sdl $v0, 0x7($sp_slot)  */
    /* m2c-unknown:  unknown instruction: sdr $v0, ($sp_slot)  */
    /* m2c-unknown:  unknown instruction: sdl $v1, 0xf($sp_slot)  */
    /* m2c-unknown:  unknown instruction: sdr $v1, 0x8($sp_slot)  */
    /* m2c-unknown:  unknown instruction: sdl $a1, 0x17($sp_slot)  */
    temp_22_115 = func_001F44B8(var_19_14, 0 /*  unknown instruction: sdr $a1, 0x10($sp_slot)  */, 4);
    if (*(s32 *)arg0->unk34 != 0) {
        var_20_120 = 0;
        do {
            var_19_124 = 0;
            if (sp40 != 0) {
                var_19_124 = ((arg0->unk40 ^ var_21_86) != 0) ? 0 : 1;
            }
            if (arg0->unk30 & 2) {
                var_16_139 = 0x80FFA888;
            } else {
                if (var_19_124 != 0) {
                    var_16_145 = 0x80006060;
                    var_2_147 = 0x8020FFFF;
                } else {
                    var_16_145 = 0x80303030;
                    var_2_147 = 0x80FFA888;
                }
                var_16_139 = (*(s32 *)((u8 *)(var_20_120 + arg0->unk34) + 0x2) != 0) ? var_2_147 : var_16_145;
            }
            if (!(arg0->unk30 & 0x10000) && (var_19_124 != 0) && ((s16) spA < 4)) {
                arg0->unk44 = (s32) ((s32) arg0->unk44 - 4);
            }
            temp_3_176 = arg0->unk30;
            temp_18_177 = func_001FDD10(*(s32 *)(var_20_120 + arg0->unk34));
            var_4_180 = 4;
            if (temp_3_176 & 0xA00) {
                var_4_180 = 0x20;
            }
            sp8 = var_4_180;
            if (temp_3_176 & 0x400) {
                sp12 = 1;
                sp8 = (s16) ((s32) arg0->unk20 >> 1);
            }
            if (var_19_124 != 0) {
                func_001F61F8(var_4_180);
            }
            func_001F7090(sp_slot, var_16_139, temp_18_177, -1, temp_22_115, sp44);
            if (var_19_124 != 0) {
                func_001F61E8();
            }
            if (arg0->unk30 & 0x200) {
                func_0021F8E8(0xF, (s16) spA + 9, *(s32 *)(var_21_86 + D_0013D408) != 0);
            }
            if (arg0->unk30 & 0x800) {
                func_0021F8E8(0xF, (s16) spA + 9, D_0015ED88 == *(s32 *)((u8 *)(var_20_120 + arg0->unk34) + 0x4));
            }
            temp_2_244 = spA + spE;
            spA = temp_2_244;
            temp_4_247 = *(s32 *)((u8 *)(var_20_120 + arg0->unk34) + 0x8);
            var_2_249 = temp_2_244;
            if (temp_4_247 != 0) {
                temp_6_254 = func_001FDD10(temp_4_247);
                sp8 = 0x14;
                func_001F7090(sp_slot, var_16_139, temp_6_254, -1, temp_22_115, sp44);
                temp_2_262 = spA + var_23_71;
                spA = temp_2_262;
                var_2_249 = temp_2_262;
            }
            temp_6_266 = arg0->unk30;
            temp_3_267 = var_2_249 + 8;
            spA = temp_3_267;
            if (!(temp_6_266 & 0x10000) && (var_19_124 != 0)) {
                temp_5_279 = temp_3_267 + (s16) spE;
                if ((s16) sp2 < temp_5_279) {
                    if (temp_6_266 & 0x8000) {
                        var_2_288 = (s32) arg0->unk44 + (temp_5_279 - (s16) sp2);
                    } else {
                        var_2_288 = (s32) arg0->unk44 + 4;
                    }
                    arg0->unk44 = var_2_288;
                }
            }
            var_20_120 += 0xC;
            var_21_86 += 1;
        } while (*(s32 *)(var_20_120 + arg0->unk34) != 0);
    }
    func_001F4398();
    temp_3_304 = arg0->unk30;
    if (!(temp_3_304 & 0x8000)) {
        return 2;
    }
    arg0->unk30 = (s32) (temp_3_304 ^ 0x8000);
    return 1;
}

extern s32 func_0021D4A8(struct M2c_arg0 *arg0) __attribute__((alias("FUN_0021d4a8")));
#endif /* NON_MATCHING */
