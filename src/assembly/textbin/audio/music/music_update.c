#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/audio/music/music_update/FUN_00216290.s", FUN_00216290);
#else
#include "rnc/audio_music_music_update_types.h"
#include "types.h"






extern struct M2c_D_001516D0 D_001516D0;
extern u8 D_001516FC[];
extern u8 D_00151704[];
extern u8 D_00216B68[];
extern void func_0012E4C0();
extern void func_0012ECA0();
extern s32 func_0012EE08(s32);
extern void func_001F96F8();
extern s32 func_001F9740();
extern void func_00215970();
extern void func_00215B68();
extern void func_00215C40();
extern void func_00215D18();
extern s32 func_00215E00();
extern void func_002160A8();
extern s32 func_00216788(s32, s32, s32);
void music_update(void) __asm__("FUN_00216290");

void music_update(void) {
    s32 *var_17_47;
    s16 temp_16_280;
    s32 temp_4_190;
    s32 temp_lo_176;
    s8 temp_4_68;
    s8 temp_5_77;
    struct M2c_temp_17_273 *temp_17_273;
    struct M2c_temp_17_66 *temp_17_66;

    if (D_001516D0.unkB != 0) {
        goto block_77;
    }
    if (D_001516D0.unk40 & 0x8000) {
        goto block_14;
    }
    if (D_001516D0.unk3E & 0x8000) {
        goto block_14;
    }
    if (D_001516D0.unk34 != 0) {
        goto block_8;
    }
    if (!(D_001516D0.unk3C & 1)) {
        goto block_7;
    }
    if (D_001516D0.unk22 != -1) {
        goto block_7;
    }
    func_00215C40(D_001516D0.unk38, (s16) D_001516D0.unk3C, D_001516D0.unk3A);
    goto block_13;
block_7:
block_8:
    if (D_001516D0.unk3E == 9) {
        goto block_14;
    }
    var_17_47 = D_001516FC;
    if (D_001516D0.unk34 == 0) {
        goto block_15;
    }
    if (D_001516D0.unk34 == 0xFFFFFFFF) {
        goto block_13;
    }
    if (D_001516D0.unk3E != 8) {
        goto block_14;
    }
    func_00215D18(D_001516D0.unk38, (s16) D_001516D0.unk3C, D_001516D0.unk3A);
block_13:
block_14:
    var_17_47 = D_001516FC;
block_15:
    temp_17_66 = var_17_47 - 0x2C;
    if (func_001F9740(var_17_47) == 0) {
        goto block_24;
    }
    temp_4_68 = temp_17_66->unk22;
    if (temp_4_68 == -1) {
        goto block_24;
    }
    if (temp_17_66->unk20 != 0) {
        goto block_24;
    }
    if (temp_17_66->unk38 != temp_4_68) {
        goto block_20;
    }
    temp_17_66->unk22 = -1;
    goto block_24;
block_20:
    temp_5_77 = temp_17_66->unk23;
    if (temp_5_77 == -1) {
        goto block_23;
    }
    if (func_00215E00(temp_4_68, temp_5_77, temp_17_66->unk3C, temp_17_66->unk3A) == 0) {
        goto block_24;
    }
block_23:
    func_001F96F8(7);
    temp_17_66->unk2C = 60.0f;
block_24:
    if (D_001516D0.unk1C < 0) {
        goto block_29;
    }
    if (D_001516D0.unk50 == 0) {
        goto block_28;
    }
    if ((u32) (D_001516D0.unk5A - 6) < 2U) {
        goto block_30;
    }
    D_001516D0.unk5A = 5U;
    goto block_29;
block_28:
    func_00215970(D_001516D0.unk1C, 0, 0x400);
    D_001516D0.unk1C = -1;
block_29:
block_30:
    if (D_001516D0.unk3E == 9) {
        goto block_68;
    }
    if (D_001516D0.unk34 == 0xFFFFFFFF) {
        goto block_68;
    }
    if (D_001516D0.unk40 & 0x8000) {
        goto block_68;
    }
    if (D_001516D0.unk3E & 0x8000) {
        goto block_68;
    }
    if (D_001516D0.unk78 & 0x8000) {
        goto block_68;
    }
    if (D_001516D0.unk76 & 0x8000) {
        goto block_68;
    }
    if (D_001516D0.unk20 == 3) {
        goto block_55;
    }
    if (D_001516D0.unk20 >= 4) {
        goto block_42;
    }
    if (D_001516D0.unk20 != 2) {
        goto block_40;
    }
    goto block_45;
block_40:
    goto block_68;
block_42:
    if (D_001516D0.unk20 == 4) {
        goto block_59;
    }
    if (D_001516D0.unk20 == 5) {
        goto block_65;
    }
    goto block_68;
block_45:
    temp_lo_176 = (s32) (D_001516D0.unk3A * (D_001516D0.unk28 - (D_001516D0.unk24 - D_001516D0.unk84))) / (s32) D_001516D0.unk28;
    if (temp_lo_176 <= 0) {
        goto block_52;
    }
    if (D_001516D0.unk76 != 4) {
        goto block_48;
    }
    if (D_001516D0.unk7C != 0) {
        goto block_49;
    }
block_48:
    if (D_001516D0.unk6C == 0) {
        goto block_52;
    }
block_49:
    temp_4_190 = D_001516D0.unk34;
    if (temp_4_190 == 0) {
        goto block_51;
    }
    goto block_53;
block_51:
block_52:
    D_001516D0.unk20 = 3;
    D_001516D0.unk3E = 5;
    goto block_68;
block_53:
    if (D_001516D0.unk3E == 9) {
        goto block_68;
    }
    D_001516D0.unk34 = 0xFFFFFFFF;
    func_0012E4C0(temp_4_190, 5, temp_lo_176, 0, 0, 0, D_00216B68, ((u8 *)&D_001516D0 + 0x34));
    goto block_68;
block_55:
    if (D_001516D0.unk3E != 0) {
        goto block_68;
    }
    if (D_001516D0.unk7C != 0) {
        goto block_58;
    }
    if (D_001516D0.unk6C != 0) {
        goto block_68;
    }
block_58:
    func_00215B68(D_001516D0.unk22, (s16) D_001516D0.unk3C, D_001516D0.unk3A, D_001516D0.unk76);
    D_001516D0.unk20 = 4;
    D_001516D0.unk22 = -1;
    goto block_68;
block_59:
    if (D_001516D0.unk3E != 3) {
        goto block_68;
    }
    if (D_001516D0.unk84 < D_001516D0.unk28) {
        goto block_64;
    }
    if (D_001516D0.unk76 != 4) {
        goto block_64;
    }
    if (D_001516D0.unk7C != 0) {
        goto block_68;
    }
    if (D_001516D0.unk6C != 0) {
        goto block_69;
    }
block_64:
    func_0012ECA0(D_001516D0.unk34, 4, 3, D_001516D0.unk76);
    D_001516D0.unk20 = 5;
    D_001516D0.unk3E = 8;
    goto block_68;
block_65:
    if (D_001516D0.unk76 != 4) {
        goto block_67;
    }
    if (D_001516D0.unk7C != 0) {
        goto block_68;
    }
block_67:
    D_001516D0.unk20 = 0;
block_68:
block_69:
    temp_17_273 = ((u8 *)D_00151704 - (u8 *)0x34);
    func_002160A8(D_00151704);
    func_002160A8(D_00151704 + 0x38);
    func_002160A8(D_00151704 + 0x1C);
    if (temp_17_273->unkA != 0) {
        goto block_71;
    }
    temp_16_280 = temp_17_273->unk8;
    goto block_74;
block_71:
    if (func_0012EE08(1) != 0) {
        goto block_78;
    }
    temp_17_273->unkA = 0U;
    temp_17_273->unk8 = 0;
    return;
block_74:
    if (temp_16_280 != 2) {
        goto block_78;
    }
    temp_17_273->unk8 = 0;
    func_00216788(temp_17_273->unk14, temp_17_273->unkC, temp_17_273->unk10);
    if (temp_17_273->unk8 != 0) {
        goto block_77;
    }
    temp_17_273->unk8 = temp_16_280;
block_77:
block_78:
    return;
}
#endif /* NON_MATCHING */
