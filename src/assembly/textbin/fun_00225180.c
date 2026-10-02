#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00225180/FUN_00225180.s", FUN_00225180);
#else
#include "eetypes.h"
struct M2c_arg0 {
    u8 pad_0[0x10];
    s64 unk10;
    u8 pad_18[0xC];
    struct M2c_arg0_unk24 * unk24;
    u8 pad_28[0x4];
    s32 unk2C;
    u8 pad_30[0x48];
    s32 unk78;
    u8 pad_7C[0x40];
    u8 unkBC;
    u8 pad_BD[0x3];
};

struct M2c_arg0_unk24 {
    u8 pad_0[0x24];
    s32 unk24;
};

struct M2c_temp_17_18 {
    u8 pad_0[0x10];
    s32 unk10;
};

extern s32 D_0015ED6C[];
extern s32 D_0015ED70;
extern s32 D_0015F438;
extern u8 D_001D5E90[];
extern u8 D_001D5EA8[];
extern s32 func_001F99F8();
extern s32 func_001F9A10();
extern s32 func_001F9CF8();
extern s32 fast_cos() __asm__("func_001F9DC8");
extern s32 fast_sin() __asm__("func_001F9DE0");
extern s32 func_001FA030();
extern s32 func_001FA2B8();
extern s32 func_001FA328();
extern s32 func_001FA580();
extern s32 func_001FA610();
extern s32 func_001FA6C0();
extern s32 func_0020D580();
extern s32 func_0020DEF8();
extern s32 func_0020E098();
extern s32 func_00213F38();
extern s32 func_00214128();
void FUN_00225180(struct M2c_arg0 *arg0) {
    u8 sp_slot[0xC0]; s32 sp10;
    s32 sp40;
    u32 sp48;
    f32 *temp_4_126;
    f32 temp_f20_111;
    f32 temp_f20_70;
    f32 temp_f21_89;
    f32 temp_f22_105;
    u32 temp_f22_124;
    u32 temp_f24_118;
    f32 temp_f24_84;
    s32 temp_3_125;
    s32 temp_hi_74;
    s32 var_hi_44;
    s32 *temp_16_25;
    struct M2c_temp_17_18 *temp_17_18;
    s32 *temp_4_171;
    s32 *temp_4_188;
    temp_17_18 = *(s32 *)((u8 *)(*(s32 *)arg0->unk78) + 0x44);
    func_0020D580();
    func_0020DEF8(arg0);
    arg0->unk10 = (s64) temp_17_18->unk10;
    temp_16_25 = ((u8 *)arg0) + 0xC0;
    func_001FA2B8(temp_16_25, ((u8 *)temp_17_18) + 0xC0);
    func_00214128(temp_16_25);
    arg0->unk2C = (f32) arg0->unk24->unk24;
    if ((u8) arg0->unkBC < 3U) {
        var_hi_44 = (s32) (arg0->unkBC * 2) % 6;
    } else {
        var_hi_44 = (s32) ((arg0->unkBC * 2) + 1) % 6;
    }
    temp_f20_70 = (f32) var_hi_44;
    temp_hi_74 = (s32) *(s32 *)0x15F438 % 200;
    temp_f24_84 = (((f32) temp_hi_74 / func_001FA6C0(0xC8)) * 6.28318f) - 3.14159f;
    temp_f21_89 = ((temp_f20_70 * 6.28318f) / func_001FA6C0(6, temp_hi_74)) - 3.14159f;
    temp_f22_105 = (((f32) ((s32) *(s32 *)0x15F438 % 170) / func_001FA6C0(0xAA)) * 6.28318f) - 3.14159f;
    temp_f20_111 = (temp_f20_70 * 12.56636f) / func_001FA6C0(6, *(s32 *)0x15F438);
    func_001FA610(temp_f20_111);
    temp_f24_118 = func_001FA580(temp_f24_84, temp_f21_89);
    __asm__ volatile ("" : "+r" (temp_f24_118));
    temp_f22_124 = func_001FA580(temp_f22_105, temp_f20_111);
    temp_3_125 = arg0->unkBC * 4;
    temp_4_126 = temp_3_125 + D_001D5E90;
    if (*temp_4_126 != 0.0f) {
        func_00213F38(temp_4_126, temp_3_125 + D_001D5EA8, 0, 0x3F800000, D_0015ED70 * 6.0f, D_0015ED6C[0] * 6.0f);
    }
    func_001F9CF8(sp_slot, sp_slot, ((u8 *)temp_17_18) + 0xC0, (fast_sin(temp_f22_124, fast_sin(temp_f24_118, fast_cos(temp_f24_118)), 0.0f) * 0.25f) + 0.5f + *(s32 *)((arg0->unkBC * 4) + D_001D5E90));
    temp_4_171 = ((u8 *)arg0) + 0x10;
    func_001F9A10(temp_4_171, temp_4_171, sp_slot);
    func_001F99F8(&sp40);
    sp48 = func_001FA580(temp_f24_118, 1.5707964f);
    func_001FA030(&sp10, &sp40);
    temp_4_188 = ((u8 *)arg0) + 0xC0;
    func_001FA328(temp_4_188, &sp10, temp_4_188);
    func_0020E098(arg0);
}

extern void func_00225180(struct M2c_arg0 *arg0) __attribute__((alias("FUN_00225180")));
#endif /* NON_MATCHING */
