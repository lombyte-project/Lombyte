#include "types.h"

struct M2c_D_001D5BF0
{
  u8 pad_0[0xA8];
  s32 unkA8;
  s32 unkAC;
};
struct M2c_var_21_27
{
  s32 unk0;
  s32 unk4;
};
extern u8 D_001B3200[];
extern u8 D_001B3AC0[];
extern u8 D_001D59D8[];
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern u8 D_001D5CA0[];
extern u8 D_001D5D38[];
extern s32 LookupResourceEntry();
extern void relocate_asset_entry_pointers() __asm__("FUN_002032e0");
extern s32 func_0020B618();
extern s32 func_00225D88();
extern s32 func_00232F20();
void FUN_00226848(s32 arg0, s32 arg1)
{
  s32 sp0;
  register s32 *temp_3_76 asm("v1");
  s32 temp_16_63;
  s32 temp_17_58;
  s32 temp_18_51;
  s32 temp_19_47;
  s32 temp_22_38;
  s32 temp_3_54;
  s32 var_16_32;
  register s32 var_20_10 asm("s4");
  s32 var_23_26;
  u8 temp_30_34;
  s32 *var_21_27;
  sp0 = arg1;
  var_20_10 = 0;
  D_001D5BF0.unkAC = sp0;
  D_001D5BF0.unkA8 = arg0;
  if (sp0 > 0)
  {
    var_23_26 = arg0 * 4;
    var_21_27 = (s32 *) (D_001D59D8 + (arg0 * 8));
    do
    {
      var_16_32 = 0;
      temp_30_34 = D_001B3AC0[var_21_27[0]];
      temp_22_38 = var_21_27[1];
      if (sp0 == 2)
      {
        if (var_20_10 == 1)
        {
          var_16_32 = 1;
        }
        else
        {
          var_16_32 = 0;
        }
      }
      var_21_27 += 2;
      temp_19_47 = *((s32 *) (D_001D5D38 + var_23_26));
      var_23_26 += 4;
      temp_18_51 = LookupResourceEntry(temp_19_47) * 0x10;
      temp_3_54 = (var_20_10 + var_16_32) * 4;
      var_20_10 += 1;
      temp_17_58 = *((s32 *) (D_001D5CA0 + temp_3_54));
      temp_16_63 = (temp_17_58 + func_00225D88(temp_17_58)) - temp_18_51;
      func_00232F20(temp_16_63, temp_19_47, 0, -1, 0);
      func_0020B618(temp_16_63, temp_17_58);
      {
        register s32 off asm("a0") = temp_22_38 * 4;
        temp_3_76 = (temp_30_34 * 4) + D_001B3200;
        *((s32 *) (((u8 *) ((*temp_3_76) + off)) + 0x48)) = temp_17_58;
      }
      relocate_asset_entry_pointers(*temp_3_76, temp_22_38);
    }
    while (var_20_10 < sp0);
  }
}
