#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00204428/FUN_00204428.s", FUN_00204428);
#else
#include "rnc/assembly_textbin_fun_00204428_types.h"
#include "types.h"






extern struct M2c_D_00137B80 D_00137B80;
extern s16 D_0013E056;
extern s32 D_0015ED58;
extern u8 D_0015ED5C[];
extern s32 D_0015ED80[];
extern u16 D_0015EE48;
extern s16 D_0015EE4A;
extern struct M2c_D_0015EE4C *D_0015EE4C;
extern s32 D_0015EE50;
extern s32 D_0015EE54;
extern s32 D_0015EEBC;
extern s32 D_0015EEC0;
extern void FUN_0022dd78();
extern u8 D_1FF8000[];
extern s32 func_0012DC80();
extern s32 func_0012E088();
extern s32 func_0012E1A8();
extern s32 func_0012E1D8();
extern s32 func_0012E3B8();
extern s32 func_00216728();
extern s32 sceCdBreak();
extern s32 sceCdGetError();
extern s32 sceCdSync();
extern void jtbl_001E7CE0();
s32 FUN_00204428(void) {
    s32 temp_16_10;
    s32 temp_2_16;
    s32 temp_4_15;
    s32 temp_4_67;
    s32 var_2_79;
    s32 var_5_106;
    s32 var_6_95;
    u16 temp_3_26;
    u16 temp_3_50;
    s32 *temp_2_107;
    s32 *temp_3_103;
    s32 *var_4_115;
    struct M2c_var_7_98 *var_7_98;

    temp_16_10 = D_0013E056 + 1;
    if (sceCdSync(1) != 0) {
        temp_4_15 = D_0015ED58;
        temp_2_16 = D_0015EEBC + 1;
        D_0015EEBC = temp_2_16;
        if (temp_4_15 == 1) {
            if (temp_2_16 >= 0x2D1) {
                D_0015EEC0 = temp_4_15;
                temp_3_26 = D_0015EE48 - 1;
                D_0015ED58 = 0;
                if ((u32) (temp_3_26 & 0xFFFF) < 3U) {
                    D_0015EE48 = temp_3_26;
                }
                sceCdBreak(temp_4_15);
            }
        }
        /* Duplicate return node #36. Try simplifying control flow for better match */
        return 0;
    }
    if (sceCdGetError() != 0) {
        if (D_0015EEC0 == 0) {
            D_0015EEC0 = 1;
            temp_3_50 = D_0015EE48 - 1;
            D_0015ED58 = 0;
            if ((u32) (temp_3_50 & 0xFFFF) < 3U) {
                D_0015EE48 = temp_3_50;
            }
        }
    }
    if ((u32) (s16) D_0015EE48 < 8U) {
        temp_4_67 = *(((u32) (s16) D_0015EE48 * 4) + &jtbl_001E7CE0);
        switch ((u32) (s16) D_0015EE48) {
        case 0:
            if (D_0015ED80[0] != 0) {
                var_2_79 = D_00137B80.unk297C;
            } else {
                var_2_79 = D_00137B80.unk2974;
            }
            var_6_95 = D_00137B80.unk296C;
            var_7_98 = D_1FF8000 - (((var_2_79 << 0xB) + 0xFFF) & 0xFFFFF000);
            temp_3_103 = ((u8 *)var_7_98) - (((*(s32 *)((u8 *)(((u8 *)&D_00137B80) + (temp_16_10 * 8)) + 0x12CC) << 0xB) + 0xFFF) & 0xFFFFF000);
            var_5_106 = D_00137B80.unk2968;
            temp_2_107 = temp_3_103 - (((var_6_95 << 0xB) + 0xFFF) & 0xFFFFF000);
            D_0015EE54 = temp_3_103;
            D_0015EE50 = var_7_98;
            D_0015EE4C = temp_2_107;
            var_4_115 = temp_2_107;
block_18:
            func_00216728(var_4_115, var_5_106, var_6_95, var_7_98);
            D_0015EE48 += 1;
            goto block_35;
        case 1:
            var_4_115 = D_0015EE54;
            var_7_98 = ((u8 *)&D_00137B80) + (temp_16_10 * 8);
            var_6_95 = var_7_98->unk12CC;
            var_5_106 = var_7_98->unk12C8;
            goto block_18;
        case 2:
            if (D_0015ED80[0] != 0) {
                func_00216728(D_0015EE50, D_00137B80.unk2978, D_00137B80.unk297C);
            } else {
                func_00216728(D_0015EE50, D_00137B80.unk2970, D_00137B80.unk2974);
            }
block_31:
            D_0015EE48 += 1;
            goto block_35;
        case 3:
            if (D_0015EE4A == 0) {
                func_0012E3B8(temp_4_67);
                if (*(s32 *)D_0015ED5C == 0) {
                    D_0015EE48 = 6;
                } else {
                    goto block_31;
                }
            }
            /* Duplicate return node #36. Try simplifying control flow for better match */
            return 0;
        case 4:
            if (func_0012DC80(temp_4_67) == 0) {
                func_0012E1D8(*(s32 *)D_0015ED5C);
                *(s32 *)D_0015ED5C = 0;
                goto block_31;
            }
            /* Duplicate return node #36. Try simplifying control flow for better match */
            return 0;
        case 5:
            if (func_0012DC80(temp_4_67) == 0) {
                func_0012E1A8();
                goto block_31;
            }
            /* Duplicate return node #36. Try simplifying control flow for better match */
            return 0;
        case 6:
            if (func_0012DC80(temp_4_67) == 0) {
                *(s32 *)D_0015ED5C = 0xFFFFFFFF;
                func_0012E088(D_0015EE4C->unk8 + D_0015EE4C, FUN_0022dd78, (u64) ((s64) D_0015ED5C << 0x20) >> 0x20);
                goto block_31;
            }
            /* Duplicate return node #36. Try simplifying control flow for better match */
            return 0;
        case 7:
            if ((func_0012DC80(temp_4_67) == 0) && (*(s32 *)D_0015ED5C != 0xFFFFFFFF)) {
                func_0012E1A8();
                return 1;
            }
            /* Duplicate return node #36. Try simplifying control flow for better match */
            return 0;
        }
    } else {
block_35:
        return 0;
    }
}

extern s32 func_00204428(void) __attribute__((alias("FUN_00204428")));
#endif /* NON_MATCHING */
