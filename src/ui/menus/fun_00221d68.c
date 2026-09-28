#include "types.h"

struct M2c_D_0013C940
{
  u8 pad_0[0x1A4];
  s32 unk1A4;
  u8 pad_1A8[0x1C];
  s32 unk1C4;
};
struct M2c_D_001D5BF0
{
  u8 pad_0[0x4];
  struct M2c_D_001D5BF0_unk4 *unk4;
  s32 unk8;
  u8 pad_C[0x118];
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
  u8 pad_18[0x28];
  s32 unk40;
};
extern struct M2c_D_0013C940 D_0013C940;
extern struct M2c_D_001D5BF0 D_001D5BF0;
__attribute__((section(".data"))) extern s32 D_001D5D14;
extern s32 func_0022DA68();
s32 FUN_00221d68(struct M2c_arg0 *arg0)
{
  s32 temp_3_27;
  s32 temp_5_41;
  register s32 var_2_44 __asm__("v0");
  if (D_0013C940.unk1C4 & 0xD00)
  {
    if (D_001D5D14 == 0)
    {
      return 1;
    }
    goto block_4;
  }
  block_4:
  if (D_0013C940.unk1C4 & 0x10)
  {
    temp_3_27 = D_001D5BF0.unk4->unk38;
    temp_5_41 = temp_3_27 != 0;
    if (temp_5_41)
    {
      D_001D5BF0.unk8 = temp_3_27;
      goto block_10;
    }
    if (D_001D5BF0.unk124 == 0)
    {
      return -1;
    }
    goto block_11;
  }

  block_10:
  block_11:
  temp_5_41 = arg0->unk40;


  if (D_0013C940.unk1A4 & 0x40)
  {
    var_2_44 = temp_5_41 + 1;
    arg0->unk40 = (s32) (var_2_44 % 30);
    goto block_after;
  }
  if (D_0013C940.unk1A4 & 0x20)
  {
    var_2_44 = temp_5_41 + 0x1D;
    arg0->unk40 = (s32) (var_2_44 % 30);
  }
  block_after:
  if (arg0->unk40 != temp_5_41)
  {
    func_0022DA68(1, 0x11, arg0->unk14);
  }
  return 0;
}
