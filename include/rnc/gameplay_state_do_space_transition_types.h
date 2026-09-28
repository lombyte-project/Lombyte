#ifndef RNC_GAMEPLAY_STATE_DO_SPACE_TRANSITION_TYPES_H
#define RNC_GAMEPLAY_STATE_DO_SPACE_TRANSITION_TYPES_H

#include "types.h"

struct M2c_D_0013D290
{
  u8 pad_0[0xD4];
  s32 unkD4;
  u8 pad_D8[0x4];
  s32 unkDC;
};

struct M2c_D_0013DD40
{
  u8 pad_0[0x8];
  u8 unk8;
  u8 pad_9[0x5];
  u8 unkE;
  u8 unkF;
  u8 pad_10[0x3];
};

struct M2c_D_0013DD58
{
  u8 unk0;
  u8 unk1;
};

struct M2c_D_0013E030
{
  u8 pad_0[0x26];
  s16 unk26;
  u8 pad_28[0x2];
};

#include "rnc/d_001516d0.h"

struct M2c_D_0015F634
{
  u8 pad_0[0x1C];
  s32 unk1C;
};

struct M2c_D_0018CD00
{
  u8 pad_0[0x218];
  s32 unk218;
  f32 unk21C;
  u8 pad_220[0x8];
  f32 unk228;
  f32 unk22C;
  s32 unk230;
  s32 unk234;
  s32 unk238;
};

struct M2c_D_00194100
{
  u8 pad_0[0x10];
  s32 unk10;
};

#endif /* RNC_GAMEPLAY_STATE_DO_SPACE_TRANSITION_TYPES_H */
