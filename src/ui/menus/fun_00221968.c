#include "types.h"

struct M2c_D_001D5BF0
{
  u8 pad_0[0x4];
  struct M2c_D_001D5BF0_unk4 *unk4;
  s32 unk8;
  s32 unkC;
  u8 pad_10[0xD4];
  s32 unkE4;
  u8 pad_E8[0x8];
  s32 unkF0;
  s32 unkF4;
  u8 pad_F8[0x2C];
  s32 unk124;
};
struct M2c_D_001D5BF0_unk4
{
  u8 pad_0[0x38];
  s32 unk38;
};
struct M2c_arg0
{
  u8 pad_0[0x14];
  s32 unk14;
};
extern s32 D_0013CB04[];
extern s32 D_0015ED84;
extern s32 D_001A0314[];
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern s32 func_0022DA68();
s32 FUN_00221968(struct M2c_arg0 *arg0)
{
  register struct M2c_arg0 *new_var asm("a3");
  s32 temp_3_13;

  new_var = arg0;
  if (!(D_0013CB04[0] & 0x10))
  {
    goto block_7;
  }
  temp_3_13 = D_001D5BF0.unk4->unk38;
  if (temp_3_13 != 0)
  {
    goto block_3;
  }
  goto block_5;
  block_3:
  D_001D5BF0.unk8 = temp_3_13;

  goto block_13;
  block_5:
  if (D_001D5BF0.unk124 != 0)
  {
    goto block_14;
  }

  return -1;
  block_7:
  if (!(D_0013CB04[0] & 0x800))
  {
    goto block_9;
  }

  D_001A0314[0] = D_0015ED84;
  return 1;
  block_9:
  if (!(D_0013CB04[0] & 0x40))
  {
    goto block_11;
  }

  func_0022DA68(0, 0x11, new_var->unk14, new_var);
  return 1;
  block_11:
  if (!(D_0013CB04[0] & 0x20))
  {
    goto block_13;
  }

  D_001D5BF0.unkE4 = (s32) D_001A0314[0];
  D_001D5BF0.unkF0 = (void *) D_001D5BF0.unk4;
  D_001D5BF0.unkC = 3;
  D_001D5BF0.unkF4 = 0xF;
  func_0022DA68(0, 0x11, new_var->unk14, new_var);
  block_13:
  block_14:
  return 0;
}

extern __typeof__(FUN_00221968) func_00221968 __attribute__((alias("FUN_00221968")));
