#include "types.h"

void decode_motion_vector(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) __asm__("FUN_001280a8");

void decode_motion_vector(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
  int new_var;
  s32 temp_4_5;
  int new_var3;
  int new_var2;
  s32 temp_9_6;
  s32 var_2_17;
  s32 var_4_9;
  temp_4_5 = *arg0;
  new_var = 1;
  temp_9_6 = 0x10 << arg1;
  var_4_9 = (arg4 != 0) ? (temp_4_5 >> new_var) : (temp_4_5);
  new_var3 = arg2 > 0;
  if (new_var3)
  {
    var_4_9 = (var_4_9 + new_var) + (((arg2 - new_var) << arg1) + arg3);
    var_2_17 = var_4_9 * 2;
    if (var_4_9 >= temp_9_6)
    {
      var_4_9 -= temp_9_6 * 2;
      goto block_6;
    }
  }
  else
  {
    var_2_17 = var_4_9 * 2;
    if (arg2 < 0)
    {
      new_var2 = (~arg2) << arg1;
      var_4_9 = (var_4_9 - new_var) - (new_var2 + arg3);
      if (var_4_9 < (-temp_9_6))
      {
        temp_9_6 = temp_9_6 * 2;
        new_var3 = temp_9_6;
        var_4_9 += new_var3;
      }
      block_6:
      ;

      ;
      ;
      ;
      ;
    }
  }
  if (arg2)
  {
    {
      new_var3 = arg4 != 0;
    }
    *arg0 = (new_var3) ? (var_4_9 * 2) : (var_4_9);
  }
  else
  {
    *arg0 = (arg4 != 0) ? (var_4_9 * 2) : (var_4_9);
  }
}

extern void func_001280A8(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) __attribute__((alias("FUN_001280a8")));
