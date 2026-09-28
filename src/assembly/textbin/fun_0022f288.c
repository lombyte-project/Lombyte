#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022f288/FUN_0022f288.s", FUN_0022f288);
#else
#include "rnc/fun_0022f288_types.h"
#include "types.h"








extern struct M2c_D_0013D290 D_0013D290;
extern u8 D_0013DD43;
extern struct M2c_D_0013E030 D_0013E030;
extern struct M2c_D_0013E500 D_0013E500;
extern s32 D_0015ED84;
extern s32 D_0015F438;
extern s32 D_0015F43C[];
extern s32 D_0015F620;
extern s32 D_0018CB54[];
extern s32 D_0018CC98;
extern struct M2c_D_0018CD00 D_0018CD00;
extern void AppendDmaTag(u32);
extern void func_001F2260();
extern void func_001F2D98();
extern void func_001F3868();
extern void func_001F4280();
extern void func_001F4398();
extern s32 func_001F44B8();
extern void func_001F5210();
extern void func_001F5450();
extern s32 func_001FA6D0(f32);
extern void func_001FB368();
extern void func_00200600();
extern void func_0020CC60(void);
extern void func_0020CEF8();
extern void func_0020D460(void);
extern void func_0022B288();
extern void func_0022E420();
extern void func_0022E8C8();
extern void func_0022EA08();
extern void func_002327A0();
extern void func_002337B0();
extern void func_00233980(s32, s64);
void FUN_0022f288(void) {
    f32 temp_f20_22;
    f32 temp_f21_157;
    s32 temp_3_77;
    s32 temp_4_85;
    s32 temp_hi_148;

    func_001FB368();
    func_001F2260();
    func_0020CC60();
    func_001F3868();
    temp_f20_22 = D_0018CD00.unkB0;
    D_0015F620 = -1;
    if (temp_f20_22 < 0.63f) {
        D_0018CD00.unkB0 = 0.63f;
    }
    func_001F2D98();
    func_001F2260();
    func_0022B288();
    D_0018CD00.unkB0 = temp_f20_22;
    func_001F2D98();
    func_001F2260();
    if (D_0013E030.unk58 == 4) {
        func_001F4280(1);
        func_0022E8C8();
        func_001F4398();
    }
    func_0020D460();
    AppendDmaTag(0x02080000);
    func_001F4280(1);
    if ((D_0015ED84 != 0) && ((D_0015ED84 != 1) || (D_0013DD43 != 0))) {
        func_0022E420(D_0018CC98);
    }
    temp_3_77 = D_0015ED84;
    if ((D_0013E030.unk58 == 4) && (D_0018CB54[0] >= 0x3D)) {
        temp_4_85 = (D_0018CB54[0] - 0x3C) * 2;
        func_0022EA08((temp_4_85 >= 0x81) ? 0x80 : temp_4_85);
    }
    if ((temp_3_77 != 0) && ((temp_3_77 != 1) || (D_0013DD43 != 0))) {
        func_002327A0(D_0018CC98);
    }
    if ((D_0013D290.unkD4 >= 3) || (D_0013D290.unkDC >= 0)) {
        func_00233980(0x47, 0x3004B);
        func_001F5450(0x2C, D_0013E500.unk4 - 0x60, 0x40, 0x40, 0, 0, 0x40, 0x40, (0x8080 << 0x10) | 0x8080, func_001F44B8(2));
        temp_hi_148 = (s32) D_0015F438 % 55;
        temp_f21_157 = (f32) ((D_0013E500.unk4 - 0x40) * 0x10);
        func_00200600(0x40, 0x40, func_001F44B8(3, temp_hi_148), 0x44980000, temp_f21_157, 0x43880000, 0x43880000, ((f32) temp_hi_148 * -6.2831855f) / 55.0f);
    }
    func_001F4398();
    if (D_0015F43C[0] > 0.0f) {
        if (D_0015F43C[0] > 1.0f) {
            D_0015F43C[0] = 1.0f;
        }
        func_001F5210(0, 0, 0, func_001FA6D0(D_0015F43C[0] * 128.0f));
    }
    func_002337B0(0x10);
    func_0020CEF8();
}
#endif /* NON_MATCHING */
