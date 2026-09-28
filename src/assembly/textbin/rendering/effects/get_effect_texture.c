#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/effects/get_effect_texture/FUN_001f44b8.s", FUN_001f44b8);
#else
#include "rnc/rendering_effects_get_effect_texture_types.h"
#include "types.h"




extern s32 D_0015EE74;
extern s32 D_0015F458;
extern s32 D_0015F460;
extern u8 D_0018D040[];
extern u8 D_0018D440[];
s64 get_effect_texture(s32 arg0) __asm__("FUN_001f44b8");

s64 get_effect_texture(s32 arg0) {
    s16 temp_7_14;
    s32 temp_11_27;
    s32 temp_12_22;
    s32 temp_2_19;
    s32 temp_6_24;
    s32 temp_6_61;
    s32 var_3_8;
    s64 temp_2_51;
    struct M2c_temp_10_10 *temp_10_10;
    struct M2c_temp_2_64 *temp_2_64;

    var_3_8 = arg0 * 0x10;
    temp_10_10 = var_3_8 + D_0018D440;
    if (temp_10_10->unk0 == 0) {
        temp_7_14 = temp_10_10->unkC;
        temp_2_19 = temp_7_14 - 6;
        temp_12_22 = (s32) D_0015EE74 >> 8;
        temp_6_24 = D_0015EE74 + 0x400;
        temp_11_27 = temp_6_24 >> 8;
        temp_2_51 = temp_11_27 | ((1 << ((temp_2_19 <= -1) ? 0 : temp_2_19)) << 0xE) | (((s64) ((s64) (u16) temp_10_10->unkC << 0x30) >> 0x16) | 0x01300000) | ((s64) ((s64) temp_10_10->unkE << 0x30) >> 0x12) | (((s64) temp_12_22 << 0x25) | (0x8000 << 0x13)) | (-1 << 0x3F);
        D_0015EE74 = temp_6_24 + (1 << (temp_7_14 + (s16) temp_10_10->unkE));
        temp_10_10->unk0 = temp_2_51;
        if (D_0015F458 < 0x40) {
            temp_6_61 = D_0015F458 * 0x10;
            temp_2_64 = temp_6_61 + D_0018D040;
            temp_2_64->unk6 = (s16) temp_12_22;
            temp_2_64->unk0 = (s32) (D_0015F460 + (temp_10_10->unkA * 0x10));
            temp_2_64->unk4 = 0;
            *(s32 *)((u8 *)(D_0018D040 + temp_6_61) + 0x8) = (s32) (D_0015F460 + (temp_10_10->unk8 * 0x10));
            temp_2_64->unkE = (s16) temp_11_27;
            temp_2_64->unkC = (u8) temp_10_10->unkC;
            temp_2_64->unkD = (u8) temp_10_10->unkE;
            D_0015F458 += 1;
        }
        var_3_8 = arg0 * 0x10;
    }
    return *(var_3_8 + D_0018D440);
}
#endif /* NON_MATCHING */
