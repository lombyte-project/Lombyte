#include "types.h"

struct M2c_D_0019A3E8
{
  u8 pad_0[0xC];
  s32 unkC;
  u8 pad_10[0x20];
  s32 unk30;
};
struct M2c_var_16_23
{
  u8 pad_0[0x18];
  s32 unk18;
};
extern s32 D_0015F444;
extern s32 D_0015F604;
extern s32 D_0015F680;
extern s32 D_0015F684;
extern s32 D_0015F688;
extern u8 D_001993D8[];
extern u8 D_00199428[];
extern u8 D_00199B60[];
extern struct M2c_D_0019A3E8 D_0019A3E8;
extern s32 func_001F96F8();
extern s32 func_00201200();
void update_hud(void) __asm__("FUN_001ff780");

void update_hud(void)
{
  s32 temp_2_27;
  short new_var2;
  u8 *var_16_23;
  s32 temp_16_90;
  s32 temp_2_59;
  s32 temp_2_76;
  s32 var_17_25;
  if (D_0019A3E8.unk30 == 0)
  {
    goto block_2;
  }
  D_0019A3E8.unk30 = 0;
  return;
  block_2:
  if (D_0015F444 == 0)
  {
    goto block_5;
  }

  D_0019A3E8.unk30 = 0;
  return;
  block_5:
  var_16_23 = D_00199B60;

  D_0019A3E8.unkC = 0xFFFFF0;
  var_17_25 = 0xC;
  loop_6:
  temp_2_27 = ((struct M2c_var_16_23 *) var_16_23)->unk18;

  if (temp_2_27 == 0)
  {
    goto block_8;
  }
  ((s32 (*)()) temp_2_27)(var_16_23);
  block_8:
  var_17_25 -= 1;

  var_16_23 += 0x90;
  if (var_17_25 >= 0)
  {
    goto loop_6;
  }
  if (D_0015F680 != 0)
  {
    goto block_11;
  }
  if (D_0015F684 == 0)
  {
    goto block_27;
  }
  block_11:
  if (D_0015F604 != 0)
  {
    goto block_28;
  }

  if (D_0015F680 == 0)
  {
    goto block_15;
  }
  temp_2_59 = D_0015F684 + (0x80 / func_001F96F8(8));
  D_0015F684 = temp_2_59;
  if (temp_2_59 < 0x81)
  {
    goto block_17;
  }
  D_0015F684 = 0x80;
  goto block_17;
  block_15:
  temp_2_76 = D_0015F684 - (0x80 / func_001F96F8(8));

  D_0015F684 = temp_2_76;
  if (temp_2_76 >= 0)
  {
    goto block_17;
  }
  D_0015F684 = 0;
    block_17:
  temp_16_90 = (D_0015F684 << 0x18) + 0xF0F0F0;

  func_00201200(0x100, D_0015F688, temp_16_90, D_001993D8);
  if (D_00199428[0] == 0)
  {
    goto block_20;
  }
  if (D_0015F680 < 0x3E9)
  {
    goto block_21;
  }
  func_00201200(0x100, D_0015F688, temp_16_90, D_00199428);
  block_20:
  block_21:
  if (D_0015F680 == 0)
  {
    goto block_23;
  }


  D_0015F680 -= 1;
  block_23:
  if (D_0015F680 != 0x3E8)
  {
    goto block_25;
  }

  D_0015F680 = 0;
  return;
  block_25:
  return;

  block_27:
  block_28:
  D_0015F688 = 0x64;


  return;
}

__attribute__((alias("FUN_001ff780"))) extern void func_001FF780(void);
