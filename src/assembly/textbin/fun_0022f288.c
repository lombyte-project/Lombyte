#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0022f288/FUN_0022f288.s", FUN_0022f288);
#else

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef volatile s8 vs8;
typedef volatile u8 vu8;
typedef volatile s16 vs16;
typedef volatile u16 vu16;
typedef volatile s32 vs32;
typedef volatile u32 vu32;
typedef volatile s64 vs64;
typedef volatile u64 vu64;
typedef float f32;
typedef double f64;
typedef s32 b32;
struct M2c_D_0013D290
{
  u8 pad_0[0xD4];
  s32 unkD4;
  u8 pad_D8[0x4];
  s32 unkDC;
};
struct M2c_D_0013E030
{
  u8 pad_0[0x58];
  s32 unk58;
};
struct M2c_D_0013E500
{
  u8 pad_0[0x4];
  s32 unk4;
};
struct M2c_D_0018CD00
{
  u8 pad_0[0xB0];
  f32 unkB0;
};
extern struct M2c_D_0013D290 D_0013D290;
extern u8 D_0013DD43[];
extern struct M2c_D_0013E030 D_0013E030;
extern struct M2c_D_0013E500 D_0013E500;
extern s32 D_0015ED84;
extern s32 D_0015F438;
extern f32 D_0015F43C;
extern s32 D_0015F620;
extern s32 D_0018CB54[];
extern s32 D_0018CC98[];
extern struct M2c_D_0018CD00 D_0018CD00;
extern void AppendDmaTag(u32);
extern void func_001F2260();
extern void func_001F2D98();
extern void func_001F3868();
extern void func_001F4280(s32);
extern void func_001F4398();
extern s64 func_001F44B8(s32);
extern void func_001F5210(s32, s32, s32, s32);
extern void func_001F5450(s32, s32, s32, s32, s32, s32, s32, s32, s64, s64);
extern s32 func_001FA6D0(f32);
extern void func_001FB368();
extern void FUN_00200600(s32, s32, s64, f32, f32, f32, f32, f32);
extern void func_0020CC60(void);
extern void func_0020CEF8();
extern void func_0020D460(void);
extern void func_0022B288();
extern void func_0022E420(s32);
extern void func_0022E8C8();
extern void func_0022EA08(s32);
extern void func_002327A0(s32);
extern void func_002337B0(s32);
extern void func_00233980(s32, s64);



extern struct M2c_D_0013E500 D_0013E500_far __asm__("D_0013E500") __attribute__((section(".data")));
void FUN_0022f288(void)
{
  f32 a;
  f32 b;
  f32 c;
  s32 u;
  s64 q;
  f32 fov;
  struct M2c_D_0013E500 *scr;
  s64 new_var;
  func_001FB368();
  func_001F2260();
  func_0020CC60();
  func_001F3868();
  fov = D_0018CD00.unkB0;
  D_0015F620 = -1;
  if (fov < 0.63f)
  {
    D_0018CD00.unkB0 = 0.63f;
  }
  func_001F2D98();
  func_001F2260();
  func_0022B288();
  D_0018CD00.unkB0 = fov;
  func_001F2D98();
  func_001F2260();
  if (D_0013E030.unk58 == 4)
  {
    func_001F4280(1);
    func_0022E8C8();
    func_001F4398();
  }
  func_0020D460();
  AppendDmaTag(0x02080000);
  func_001F4280(1);
  if ((D_0015ED84 != 0) && ((D_0015ED84 != 1) || (D_0013DD43[0] != 0)))
  {
    func_0022E420(D_0018CC98[0]);
  }
  if ((D_0013E030.unk58 == 4) && (D_0018CB54[0] >= 0x3D))
  {
    u = (D_0018CB54[0] - 0x3C) * 2;
    if (u >= 0x81)
    {
      u = 0x80;
    }
    func_0022EA08(u);
  }
  if ((D_0015ED84 != 0) && ((D_0015ED84 != 1) || (D_0013DD43[0] != 0)))
  {
    func_002327A0(D_0018CC98[0]);
  }
  if ((D_0013D290.unkD4 >= 3) || (D_0013D290.unkDC >= 0))
  {
    func_00233980(0x47, 0x3004B);
    c = 272.0f;
    new_var = func_001F44B8(2);
    scr = &D_0013E500_far;
    func_001F5450(0x2C, scr->unk4 - 0x60, 0x40, 0x40, 0, 0, 0x40, 0x40, 0x80808080, new_var);
    b = (scr->unk4 - 0x40) * 16;
    a = ((D_0015F438 % 55) * (-6.2831855f)) / 55.0f;
    FUN_00200600(0x40, 0x40, func_001F44B8(3), 1216.0f, b, c, c, a);
  }
  func_001F4398();
  if (D_0015F43C > 0.0f)
  {
    if (D_0015F43C > 1.0f)
    {
      D_0015F43C = 1.0f;
    }
    func_001F5210(0, 0, 0, func_001FA6D0(D_0015F43C * 128.0f));
  }
  func_002337B0(0x10);
  func_0020CEF8();
}
#endif /* NON_MATCHING */
