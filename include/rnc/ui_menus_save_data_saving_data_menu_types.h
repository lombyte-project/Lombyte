#ifndef RNC_UI_MENUS_SAVE_DATA_SAVING_DATA_MENU_TYPES_H
#define RNC_UI_MENUS_SAVE_DATA_SAVING_DATA_MENU_TYPES_H

#include "types.h"

struct M2c_D_0013C940
{
  u8 pad_0[0x1A4];
  s32 unk1A4;
  u8 pad_1A8[0xC];
  s32 unk1B4;
  u8 pad_1B8[0xC];
  s32 unk1C4;
};

struct M2c_D_0013D290
{
  u8 pad_0[0x8];
  s32 unk8;
  u8 pad_C[0x8];
  s32 unk14;
  u8 pad_18[0xBC];
  s32 unkD4;
  u8 pad_D8[0x4];
  s32 unkDC;
  u8 pad_E0[0x4];
  s32 unkE4;
};

struct M2c_D_001D5BF0
{
  u8 pad_0[0x4];
  struct M2c_D_001D5BF0_unk4 *unk4;
  s32 unk8;
  u8 pad_C[0xC4];
  struct M2c_D_001D5BF0_unkD0 *unkD0;
  s32 unkD4;
  u8 pad_D8[0x4C];
  s32 unk124;
  s32 unk128;
  s32 unk12C;
};

struct M2c_D_001D5BF0_unk4
{
  u8 pad_0[0x38];
  s32 unk38;
  u8 pad_3C[0x4];
  s32 unk40;
};

struct M2c_D_001D5BF0_unkD0
{
  u8 pad_0[0x84];
  s32 unk84;
};

struct M2c_arg0
{
  u8 pad_0[0x14];
  s32 unk14;
  u8 pad_18[0x18];
  s32 unk30;
  u8 pad_34[0xC];
  s32 unk40;
  u8 pad_44[0x4];
  s32 unk48;
  s32 unk4C;
};

#endif /* RNC_UI_MENUS_SAVE_DATA_SAVING_DATA_MENU_TYPES_H */
