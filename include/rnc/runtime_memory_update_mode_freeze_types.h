#ifndef RNC_RUNTIME_MEMORY_UPDATE_MODE_FREEZE_TYPES_H
#define RNC_RUNTIME_MEMORY_UPDATE_MODE_FREEZE_TYPES_H

#include "types.h"

struct S_0013C940
{
  u8 pad_0[0x1A4];
  s32 unk1A4;
};

struct S_0013F350_2080
{
  u8 pad_0[0x31];
  u8 unk31;
  u8 pad_31[0x2];
  u16 unk34;
  u8 pad_36[0x5E];
  s32 unk94;
};

struct S_0013F350_890
{
  u8 pad_0[0xBC];
  u8 unkBC;
};

struct S_0013F350
{
  u8 pad_0[0x19C];
  s32 unk19C;
  u8 pad_1A0[0x6E0];
  s32 unk880;
  u8 pad_884[0xC];
  struct S_0013F350_890 *unk890;
  u8 pad_894[0x6];
  s16 unk89A;
  u8 pad_89C[0xD73];
  u8 unk160F;
  u8 pad_1610[0xA70];
  struct S_0013F350_2080 *unk2080;
};

struct S_00193300
{
  s32 unk0;
  s32 unk4;
  s32 unk8;
  s32 unkC;
  s32 unk10;
  s32 unk14;
  s32 unk18;
  s32 unk1C;
  s32 unk20;
  s32 unk24;
};

#endif /* RNC_RUNTIME_MEMORY_UPDATE_MODE_FREEZE_TYPES_H */
