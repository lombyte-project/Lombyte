#include "types.h"
#include "rnc/sdk_library__decMB0_types.h"

extern u8 D_00153898[];
extern void _Error();
extern void _flushBuf();
extern s16 _ipuVdec(struct M2c_arg0 *, s32);
extern void _motionVector();
extern void _motionVectors();
extern s32 _nextBit(struct M2c_arg0 *, s32);
extern void _sendIpuCommand();
extern void _waitIpuIdle();
extern void receiveDataFromIPU();
int _decMB0(struct M2c_arg0 *arg0, int *arg1, int *arg2, int *arg3, struct M2c_arg4 *arg4, int *arg5, int arg6)
{
  int *sp20;
  int *sp24;
  u32 g1;
  u32 g2;
  int var_19_84;
  int temp_2_32;
  int temp_3_210;
  int temp_4_287;
  int new_var;
  int temp_6_241;
  int command_base;
  int temp_6_75;
  int temp_7_163;
  int temp_7_200;
  int var_20_86;
  int var_23_95;
  int var_2_103;
  int motion_flag;
  int var_2_172;
  int var_2_213;
  *((int *) 0x10002010) = ((*((int *) 0x10002010)) & 0xF8FFFFFF) | (arg0->unk150 << 0x18);
  sp20 = arg3;
  sp24 = arg5;
  temp_2_32 = _ipuVdec(arg0, 1);
  *arg1 = temp_2_32;
  if (temp_2_32 == 0)
  {
    _Error(arg0, D_00153898);
    arg0->unk11C = 1;
    return 0;
  }
  if (temp_2_32 & 0xC)
  {
    if ((arg0->unk174 == 3) && (arg0->unk17C != 0))
    {
      *arg2 = 2;
    }
    else
    {
      *arg2 = _nextBit(arg0, 2);
    }
  }
  else
    if ((temp_2_32 & 1) && (arg0->unk180 != 0))
  {
    *arg2 = ((arg0->unk174 ^ 3) == 0) ? (2) : (1);
  }
  temp_6_75 = arg0->unk174;
  if (temp_6_75 == 3)
  {
    var_19_84 = (((*arg2) ^ 1) == 0) ? (2) : (1);
    var_20_86 = (*arg2) == 2;
  }
  else
  {
    new_var = 2;
    var_20_86 = 0;
    var_19_84 = (((*arg2) ^ new_var) == 0) ? (2) : (1);
  }
  var_23_95 = 0;
  motion_flag = (*arg2) == 3;
  if (var_20_86 == 0)
  {
    var_23_95 = temp_6_75 == 3;
  }
  var_2_103 = 0;
  if (((arg0->unk174 == 3) && (arg0->unk17C == 0)) && (((*arg1) & 3) != 0))
  {
    var_2_103 = _nextBit(arg0, 1);
  }
  else
  {
    var_2_103 = 0;
  }
  *sp20 = var_2_103;
  if ((*arg1) & 0x10)
  {
    arg0->unk1B4 = _nextBit(arg0, 5);
  }
  if (((*arg1) & 8) || (((*arg1) & 1) && (arg0->unk180 != 0)))
  {
    if (arg0->unk848 != 0)
    {
      _motionVectors(arg0, arg4, arg6, sp24, 0, var_19_84, var_20_86, arg0->unk164 - 1, arg0->unk168 - 1, motion_flag, var_23_95);
    }
    else
    {
      temp_7_163 = arg0->unk158 - 1;
      _motionVector(arg0, arg4, arg6, temp_7_163, temp_7_163, 0, 0, arg0->unk154);
    }
  }
  var_2_172 = 0;
  if (arg0->unk11C == 0)
  {
    if ((*arg1) & 4)
    {
      if (arg0->unk848 != 0)
      {
        _motionVectors(arg0, arg4, arg6, sp24, 1, var_19_84, var_20_86, arg0->unk16C - 1, arg0->unk170 - 1, 0, var_23_95);
      }
      else
      {
        temp_7_200 = arg0->unk160 - 1;
        _motionVector(arg0, ((u8 *) arg4) + 8, arg6, temp_7_200, temp_7_200, 0, 0, arg0->unk15C);
      }
    }
    var_2_172 = 0;
    if (arg0->unk11C == 0)
    {
      temp_3_210 = *arg1;
      var_2_213 = temp_3_210 & 3;
      if (temp_3_210 & 1)
      {
        if (arg0->unk180 != 0)
        {
          _flushBuf(arg0, 1);
        }
        ;
      }
      if (((*arg1) & 3) != 0)
      {
        receiveDataFromIPU(*((int *) (((u8 *) (((u8 *) arg0) + (arg0->unk810 * 0x140))) + 0x594)), 0x300);
        _waitIpuIdle(arg0);
        g1 = (((*arg1) & 1) << 0x1B) | (arg0->unk1B4 << 0x10);
        g2 = ((arg0->unk1B0) << 0x1A) | 0x20000000;
        _sendIpuCommand(arg0, (g1 | g2) | ((*sp20) << 0x19));
      }
      else
      {
        *((int *) (((u8 *) (((u8 *) arg0) + (arg0->unk810 * 0x140))) + 0x6CC)) = 1;
      }
      arg0->unk1B0 = 0;
      if (arg0->unk11C != 0)
      {
        if (arg0->unk174)
        {
          return 0;
        }
        return 0;
      }
      if ((((*arg1) & 1) || ((arg0->unk1B0 = 1, ((*arg1) & 1) != 0))) && (arg0->unk180 == 0))
      {
        arg4->unk14 = 0;
        arg4->unk10 = 0;
        arg4->unk4 = 0;
        arg4->unk0 = 0;
        arg4->unk1C = 0;
        arg4->unk18 = 0;
        arg4->unkC = 0;
        arg4->unk8 = 0;
      }
      temp_4_287 = arg0->unk150;
      var_2_172 = 1;
      if (temp_4_287 == 2)
      {
        var_2_172 = 1;
        if (!((*arg1) & 9))
        {
          arg4->unk0 = (arg4->unk4 = (arg4->unk10 = (arg4->unk14 = 0)));
          if (arg0->unk174 == 3)
          {
            *arg2 = temp_4_287;
          }
          else
          {
            *arg2 = 1;
            *sp24 = arg0->unk174 == 2;
          }
          var_2_172 = 1;
        }
      }
      return var_2_172;
    }
    return var_2_172;
  }
  return var_2_172;
}
