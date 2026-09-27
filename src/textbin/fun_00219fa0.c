#include "types.h"
struct M2c_arg0
{
  u8 pad_0[0x20];
  s32 unk20;
  s32 unk24;
  u8 pad_28[0x14];
  s32 unk3C;
  s32 unk40;
  u8 pad_44[0x4];
  struct M2c_temp_22_35 *unk48;
  u8 pad_4C[0x10];
  s32 unk5C;
};
struct M2c_temp_22_35
{
  u16 unk0;
  s16 unk2;
};
extern s32 D_0015F438;
extern s32 D_001601B0;
extern s32 SubtractIntegerWithClamp();
extern void func_001F4280();
extern void func_001F4398();
extern s32 func_001FF960();
extern void func_001FFC30();
extern void func_001FFE18() __asm__("FUN_001ffe18");
extern void func_00200080();
extern void func_00200E08();

s32 FUN_00219fa0(struct M2c_arg0 *arg0)
{
  s32 new_var2;
  s32 sp0;
  int new_var;
  s32 temp_21_23;
  s32 temp_5_100;
  s32 var_18_18;
  s32 var_19_11;
  int new_var3;
  s32 var_30_28;
  struct M2c_temp_22_35 *temp_22_35;
  var_19_11 = 0;
  var_18_18 = (*arg0).unk5C;
  sp0 = var_18_18;
  temp_21_23 = ((s32) ((arg0->unk20 * 0x10) - 0x200)) >> 1;
  func_001F4280(0);
  new_var3 = 0;
  new_var = new_var3;
  if (arg0->unk40 > new_var3)
  {
    var_30_28 = new_var3;
    do
    {
      temp_22_35 = (struct M2c_temp_22_35 *) ((u8 *) arg0->unk48 + var_30_28);
      if (arg0->unk3C == var_19_11)
      {
        func_00200E08(temp_21_23 - 0x30, var_18_18 - 0x30, 0x230 + temp_21_23, var_18_18 + 0x230, (u64) (u32) (((SubtractIntegerWithClamp(((*(s32 *) 0x15F438) & 0x3F) - 0x20) + 0x40) * 0x10202) | 0x80000000), 1);
        func_00200E08(temp_21_23 - 0x10, var_18_18 - 0x10, temp_21_23 + 0x210, var_18_18 + 0x210, (u64) D_001601B0, 1);
      }
      var_19_11 = var_19_11 + 1;
      var_30_28 += 0xA;
      func_00200080(func_001FF960(temp_22_35->unk0, temp_22_35->unk2), new_var2 = temp_21_23, var_18_18, 0x200, 0x200, 0x80);
      var_18_18 += 0x260;
    }
    while (var_19_11 < arg0->unk40);
  }
  if (sp0 < new_var)
  {
    func_00200E08(new_var, new_var, arg0->unk20, 0x14, (u64) D_001601B0, new_var);
    func_001FFC30(func_001FF960(0xE99EU, 6), (temp_21_23 >> 4), 2, 0x20, 0x10, 0x80);
  }
  temp_5_100 = arg0->unk24;
  if ((temp_5_100 * 0x10) < var_18_18)
  {
    var_30_28 = new_var;
    func_00200E08(var_30_28, temp_5_100 - 0x14, arg0->unk20, temp_5_100, (u64) D_001601B0, var_30_28);
    func_001FFE18(func_001FF960(0xE99EU, 6), (temp_21_23 >> 4), arg0->unk24 - 0x12, 0x20, 0x10, 0x80);
  }
  func_001F4398();
  return 2;
}
