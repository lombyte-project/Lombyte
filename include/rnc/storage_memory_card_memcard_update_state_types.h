#ifndef RNC_STORAGE_MEMORY_CARD_MEMCARD_UPDATE_STATE_TYPES_H
#define RNC_STORAGE_MEMORY_CARD_MEMCARD_UPDATE_STATE_TYPES_H

#include "types.h"

struct M2c_D_00137B80
{
  u8 pad_0[0x10];
  s32 unk10;
  s32 unk14;
};

struct M2c_D_0013D290
{
  u8 pad_0[0xCC];
  s32 unkCC;
  u8 pad_D0[0x4];
  s32 unkD4;
  s32 unkD8;
};

struct M2c_sp48
{
  s32 unk0;
  u8 pad_4[0x4];
  s32 unk8;
  s32 unkC;
  s32 unk10;
};

struct M2c_sp58
{
  u8 pad_0[0x10];
  s32 unk10;
};

struct M2c_temp_16_1103
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0x8];
  s32 unkD0;
  u8 pad_D4[0x4];
  s32 unkD8;
  u8 pad_DC[0x14];
  s32 unkF0;
};

struct M2c_temp_16_180
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0xC];
  s32 unkD4;
  s32 unkD8;
};

struct M2c_temp_16_35
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0xC];
  s32 unkD4;
};

struct M2c_temp_17_1300
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0x10];
  s32 unkD8;
};

struct M2c_temp_17_1385
{
  u8 pad_0[0xD0];
  s32 unkD0;
  u8 pad_D4[0x4];
  s32 unkD8;
};

struct M2c_temp_17_1470
{
  u8 pad_0[0xD0];
  s32 unkD0;
  u8 pad_D4[0x4];
  s32 unkD8;
};

struct M2c_temp_3_108
{
  u8 pad_0[0xDC];
  s32 unkDC;
  s32 unkE0;
};

struct M2c_temp_3_127
{
  u8 pad_0[0x4];
  s32 unk4;
  u8 pad_8[0x8];
  s32 unk10;
};

struct M2c_temp_3_152
{
  u8 pad_0[0x4];
  s32 unk4;
  u8 pad_8[0x8];
  s32 unk10;
};

struct M2c_temp_3_419
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0xC];
  s32 unkD4;
  s32 unkD8;
};

struct M2c_temp_3_750
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0xC];
  s32 unkD4;
  s32 unkD8;
};

struct M2c_temp_4_1566
{
  u8 pad_0[0xD8];
  s32 unkD8;
  u8 pad_DC[0x10];
  s32 unkEC;
};

struct M2c_temp_4_91
{
  u8 pad_0[0x1C];
  s32 unk1C;
  u8 pad_20[0xA4];
  s32 unkC4;
  u8 pad_C8[0xC];
  s32 unkD4;
  u8 pad_D8[0x4];
  s32 unkDC;
  s32 unkE0;
  s32 unkE4;
};

struct M2c_temp_4_911
{
  u8 pad_0[0xC4];
  s32 unkC4;
  s32 unkC8;
  u8 pad_CC[0x8];
  s32 unkD4;
};

struct M2c_temp_5_212
{
  u8 pad_0[0xBC];
  s32 unkBC;
  u8 pad_C0[0x4];
  s32 unkC4;
  u8 pad_C8[0x4];
  s32 unkCC;
  u8 pad_D0[0x8];
  s32 unkD8;
  u8 pad_DC[0x8];
  s32 unkE4;
  s32 unkE8;
};

struct M2c_temp_6_517
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0x10];
  s32 unkD8;
};

struct M2c_temp_6_973
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0xC];
  s32 unkD4;
};

struct M2c_temp_7_60
{
  u8 pad_0[0xBC];
  s32 unkBC;
  u8 pad_C0[0x4];
  s32 unkC4;
  u8 pad_C8[0xC];
  s32 unkD4;
};

struct M2c_temp_7_804
{
  u8 pad_0[0x4];
  s32 unk4;
};

struct M2c_temp_7_923
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0x4];
  s32 unkCC;
  u8 pad_D0[0x4];
  s32 unkD4;
  s32 unkD8;
};

struct M2c_temp_9_47
{
  u8 pad_0[0x4];
  s32 unk4;
};

struct M2c_var_16_1055
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0x8];
  s32 unkD0;
  u8 pad_D4[0x4];
  s32 unkD8;
  u8 pad_DC[0x14];
  s32 unkF0;
};

struct M2c_var_16_1183
{
  u8 pad_0[0xC4];
  s32 unkC4;
  s32 unkC8;
  u8 pad_CC[0x4];
  s32 unkD0;
  u8 pad_D4[0x4];
  s32 unkD8;
  u8 pad_DC[0x14];
  s32 unkF0;
};

struct M2c_var_16_123
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0x4];
  s32 unkCC;
  s32 unkD0;
  s32 unkD4;
};

struct M2c_var_16_257
{
  u8 pad_0[0xBC];
  s32 unkBC;
  u8 pad_C0[0x4];
  s32 unkC4;
  s32 unkC8;
  s32 unkCC;
  s32 unkD0;
  s32 unkD4;
  s32 unkD8;
  u8 pad_DC[0x8];
  s32 unkE4;
  s32 unkE8;
  s32 unkEC;
  s32 unkF0;
};

struct M2c_var_16_307
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0x8];
  s32 unkD0;
  u8 pad_D4[0x4];
  s32 unkD8;
  u8 pad_DC[0x14];
  s32 unkF0;
};

struct M2c_var_16_773
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0x10];
  s32 unkD8;
};

struct M2c_var_17_324
{
  u8 pad_0[0xBC];
  s32 unkBC;
  u8 pad_C0[0x4];
  s32 unkC4;
  u8 pad_C8[0x4];
  s32 unkCC;
  s32 unkD0;
  s32 unkD4;
  s32 unkD8;
  u8 pad_DC[0x8];
  s32 unkE4;
  s32 unkE8;
  s32 unkEC;
  s32 unkF0;
};

struct M2c_var_2_511
{
  u8 pad_0[0xC4];
  s32 unkC4;
  u8 pad_C8[0x4];
  s32 unkCC;
  u8 pad_D0[0x4];
  s32 unkD4;
};

struct M2c_var_2_744
{
  u8 pad_0[0x14];
  s32 unk14;
  u8 pad_18[0x94];
  s32 unkAC;
  u8 pad_B0[0x14];
  s32 unkC4;
  u8 pad_C8[0x4];
  s32 unkCC;
  u8 pad_D0[0x4];
  s32 unkD4;
};

struct M2c_var_3_1373
{
  u8 pad_0[0xBC];
  s32 unkBC;
  u8 pad_C0[0x4];
  s32 unkC4;
  u8 pad_C8[0x4];
  s32 unkCC;
  u8 pad_D0[0x4];
  s32 unkD4;
  s32 unkD8;
};

struct M2c_var_3_504
{
  u8 pad_0[0xCC];
  s32 unkCC;
  u8 pad_D0[0x8];
  s32 unkD8;
};

struct M2c_var_4_141
{
  u8 pad_0[0xBC];
  s32 unkBC;
  u8 pad_C0[0x4];
  s32 unkC4;
  u8 pad_C8[0x4];
  s32 unkCC;
  u8 pad_D0[0x4];
  s32 unkD4;
  u8 pad_D8[0xC];
  s32 unkE4;
  s32 unkE8;
};

struct M2c_var_4_441
{
  u8 pad_0[0xBC];
  s32 unkBC;
  u8 pad_C0[0x4];
  s32 unkC4;
  u8 pad_C8[0xC];
  s32 unkD4;
  s32 unkD8;
  u8 pad_DC[0x8];
  s32 unkE4;
  s32 unkE8;
};

struct M2c_var_5_269
{
  u8 pad_0[0xBC];
  s32 unkBC;
  u8 pad_C0[0x4];
  s32 unkC4;
  u8 pad_C8[0x8];
  s32 unkD0;
  u8 pad_D4[0x4];
  s32 unkD8;
  u8 pad_DC[0x8];
  s32 unkE4;
  s32 unkE8;
};

struct M2c_var_5_472
{
  u8 pad_0[0xBC];
  s32 unkBC;
  u8 pad_C0[0x4];
  s32 unkC4;
  u8 pad_C8[0x10];
  s32 unkD8;
  u8 pad_DC[0x8];
  s32 unkE4;
  s32 unkE8;
};

#endif /* RNC_STORAGE_MEMORY_CARD_MEMCARD_UPDATE_STATE_TYPES_H */
