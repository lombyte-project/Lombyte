#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/rpc/sce_sif_call_rpc/sceSifCallRpc.s", sceSifCallRpc);
#else
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
struct M2c_arg0
{
  s32 unk0;
  s32 unk4;
  s32 unk8;
  u8 pad_C[0x8];
  s32 unk14;
  u8 pad_18[0x4];
  s32 unk1C;
  s32 unk20;
  s32 unk24;
};
struct M2c_temp_2_23
{
  u8 pad_0[0x14];
  s32 unk14;
  s32 unk18;
  s32 unk1C;
  s32 unk20;
  s32 unk24;
  s32 unk28;
  s32 unk2C;
  s32 unk30;
  s32 unk34;
};
extern u8 D_00156800[];
extern s32 CreateSema();
extern s32 DeleteSema();
extern s32 WaitSema();
extern s32 func_0011ACE8();
extern s32 func_0011AD90();
extern s32 sceSifSendCmd();
extern s32 sceSifWriteBackDCache();
s32 sceSifCallRpc(struct M2c_arg0 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg_sp0)
{
  s32 sema_param[6];
  struct M2c_temp_2_23 *temp_2_23;
  s32 temp_2_91;
  s32 not_sync;
  s32 new_var;
  s32 var_2_27;
  temp_2_23 = (struct M2c_temp_2_23 *) func_0011ACE8(D_00156800);
  var_2_27 = -1;
  if (temp_2_23 == 0)
  {
    return var_2_27;
  }
  new_var = temp_2_23->unk18;
  not_sync = arg2 & 2;
  arg0->unk20 = arg_sp0;
  arg0->unk0 = (s32) temp_2_23;
  arg0->unk4 = new_var;
  arg0->unk1C = arg7;
  temp_2_23->unk20 = arg1;
  temp_2_23->unk24 = arg4;
  temp_2_23->unk28 = arg5;
  temp_2_23->unk2C = arg6;
  temp_2_23->unk14 = (s32) temp_2_23;
  temp_2_23->unk34 = arg0->unk24;
  temp_2_23->unk1C = (s32) arg0;
  if (!not_sync)
  {
    if (arg3 == arg5)
    {
      sceSifWriteBackDCache(arg3, (arg4 >= arg6) ? (arg4) : (arg6));
    }
    else
    {
      if (arg4 > 0)
      {
        sceSifWriteBackDCache(arg3, arg4);
      }
      if (arg6 > 0)
      {
        sceSifWriteBackDCache(arg5, arg6);
      }
    }
  }
  if (arg2 & 1)
  {
    if (arg7 == 0)
    {
      temp_2_23->unk30 = 0;
    }
    else
    {
      temp_2_23->unk30 = 1;
    }
    arg0->unk8 = -1;
    if (sceSifSendCmd(0x8000000A, temp_2_23, 0x40, arg3, arg0->unk14, arg4) != 0)
    {
      return 0;
    }
    else
    {
      func_0011AD90(temp_2_23);
      return -2;
    }
  }
  else
  {
    sema_param[1] = 1;
    sema_param[2] = 0;
    temp_2_91 = CreateSema(sema_param);
    arg0->unk8 = temp_2_91;
    if (temp_2_91 < 0)
    {
      func_0011AD90(temp_2_23);
      return -3;
    }
    temp_2_23->unk30 = 1;
    if (sceSifSendCmd(0x8000000A, temp_2_23, 0x40, arg3, arg0->unk14, arg4) == 0)
    {
      DeleteSema(arg0->unk8);
      func_0011AD90(temp_2_23);
      return -2;
    }
    WaitSema(arg0->unk8);
    DeleteSema(arg0->unk8);
    return 0;
  }
}

#endif /* NON_MATCHING */
