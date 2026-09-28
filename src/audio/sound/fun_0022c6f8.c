#include "types.h"

struct M2c_arg0
{
  u8 pad_0[0x8];
  s32 unk8;
  s32 unkC;
  u8 pad_10[0x9];
  u8 unk19;
};
extern f32 func_001FA6C0();
extern s32 truncate_float_to_s32(f32) __asm__("FUN_001fa6d0");
s32 FUN_0022c6f8(struct M2c_arg0 *arg0, f32 fparg0, f32 fparg1, f32 fparg2)
{
  f32 temp_f0_26;
  if (!(arg0->unk19 & 1))
  {
    goto block_8;
  }
  if (fparg0 <= fparg1)
  {
    return arg0->unkC;
  }
  if (fparg2 <= fparg0)
  {
    return arg0->unk8;
  }
  temp_f0_26 = func_001FA6C0(arg0->unkC - arg0->unk8);
  return arg0->unk8 + truncate_float_to_s32(((fparg2 - fparg0) * (fparg2 - fparg0) * temp_f0_26) / ((fparg2 - fparg1) * (fparg2 - fparg1)));
  block_8:
  if (fparg0 <= fparg1)
  {
    return arg0->unkC;
  }
  if (fparg2 <= fparg0)
  {
    return arg0->unk8;
  }
  temp_f0_26 = func_001FA6C0(arg0->unkC - arg0->unk8);
  return arg0->unk8 + truncate_float_to_s32(((fparg2 - fparg0) * temp_f0_26) / (fparg2 - fparg1));
}
