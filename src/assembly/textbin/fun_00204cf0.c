#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00204cf0/FUN_00204cf0.s", FUN_00204cf0);
#else
#include "rnc/fun_00204cf0_types.h"
#include "types.h"




extern u32 D_0015EE74[];
extern u32 D_0015F458[];
extern u8 D_0018D040[];
extern s32 func_001F97A0();
s32 FUN_00204cf0(struct M2c_arg0 *arg0) {
u8 *sp0;
    s32 *sp4;
    s32 sp14;
    s32 sp4C;
    s32 sp50;
    s32 temp_11_31;
    s32 temp_12_26;
    s32 temp_2_16;
    s32 temp_3_23;
    s32 temp_4_64;
    s32 temp_7_28;
    s32 temp_8_56;
    s32 *temp_16_55;
    struct M2c_temp_2_66 *temp_2_66;

    sp14 = 0x400;
    sp0 = ((u8 *)arg0 + (0x20));
    sp4C = func_001F97A0(arg0->unk8);
    temp_2_16 = func_001F97A0(arg0->unkC);
    temp_3_23 = sp4C - 6;
    temp_12_26 = (s32) D_0015EE74[0] >> 8;
    temp_7_28 = D_0015EE74[0] + 0x400;
    sp50 = temp_2_16;
    temp_11_31 = temp_7_28 >> 8;
    temp_16_55 = ((u8 *)arg0 + ((sp14 + 0x20)));
    temp_8_56 = temp_11_31 | ((1 << ((temp_3_23 <= -1) ? 0 : temp_3_23)) << 0xE) | (((s64) sp4C << 0x1A) | 0x01300000) | ((s64) temp_2_16 << 0x1E) | (((s64) temp_12_26 << 0x25) | (0x8000 << 0x13)) | (-1 << 0x3F);
    D_0015EE74[0] = temp_7_28 + (1 << (sp4C + temp_2_16));
    sp4 = temp_16_55;
    if (D_0015F458[0] < 0x40) {
        temp_4_64 = D_0015F458[0] * 0x10;
        temp_2_66 = temp_4_64 + D_0018D040;
        temp_2_66->unk0 = sp0;
        temp_2_66->unk6 = (s16) temp_12_26;
        temp_2_66->unk4 = 0;
        *(s32 *)((u8 *)(D_0018D040 + temp_4_64) + 0x8) = temp_16_55;
        temp_2_66->unkE = (s16) temp_11_31;
        temp_2_66->unkC = (u8) sp4C;
        temp_2_66->unkD = (u8) sp50;
        D_0015F458[0] += 1;
    }
    return temp_8_56;
}
#endif /* NON_MATCHING */
