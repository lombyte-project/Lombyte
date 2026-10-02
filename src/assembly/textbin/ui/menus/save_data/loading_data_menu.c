#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/save_data/loading_data_menu/FUN_002232d8.s", FUN_002232d8);
#else

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef volatile s8 vs8;
typedef volatile u8 vu8;
typedef volatile s16 vs16;
typedef volatile u16 vu16;
typedef volatile s32 vs32;
typedef volatile u32 vu32;
typedef volatile s64 vs64;
typedef volatile u64 vu64;
typedef float f32;
typedef double f64;
typedef s32 b32;
struct M2c_D_0013C940
{
  u8 pad_0[0x1B4];
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
  u8 pad_18[0xA8];
  s32 unkC0;
  u8 pad_C4[0x10];
  s32 unkD4;
  u8 pad_D8[0x4];
  s32 unkDC;
  s32 unkE0;
  s32 unkE4;
  u8 pad_E8[0xC];
  s32 unkF4;
};
struct M2c_D_0013E550
{
  u8 pad_0[0x48];
  s32 unk48;
  s32 unk4C;
  s32 unk50;
  s32 unk54;
  s32 unk58;
  s32 unk5C;
};
struct M2c_D_001D5BF0
{
  u8 pad_0[0x4];
  struct M2c_D_001D5BF0_unk4 *unk4;
  s32 unk8;
  u8 pad_C[0x118];
  s32 unk124;
  s32 unk128;
  s32 unk12C;
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
  u8 pad_18[0x18];
  s32 unk30;
  u8 pad_34[0xC];
  s32 unk40;
};
extern struct M2c_D_0013C940 D_0013C940;
extern struct M2c_D_0013D290 D_0013D290;
extern s32 D_0013E05A;
extern struct M2c_D_0013E550 D_0013E550;
extern s32 D_0015ED84;
extern s32 D_0015EDEC;
extern s32 D_0015EDF0;
extern s32 D_0015EE34;
extern s32 D_0015EEB0;
extern s32 D_0015EEB4;
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern s32 InitializeGlobalStateEntry();
extern s32 func_001FBAB8();
extern s32 func_0022DA68();
s32 loading_data_menu(struct M2c_arg0 *arg0) __asm__("FUN_002232d8");

s32 loading_data_menu(struct M2c_arg0 *arg0)
{
  s32 temp_19_15;
  s32 mobj_unk38;
  s32 sub_unk40;
  s32 flags;
  s32 e8_scale;
  s32 e7_scale;
  struct M2c_D_0013E550 *init_unk;
  temp_19_15 = arg0->unk40;
  if (D_001D5BF0.unk128 != 0)
  {
    if ((D_0013D290.unkD4 < 3) && (D_0013D290.unkDC < 0))
    {
      D_001D5BF0.unk128 = 0;
      if (D_0013D290.unkE4 != 0)
      {
        D_0015EEB4 |= 0x100;
        func_001FBAB8(3, D_001D5BF0.unk4);
        return 0;
      }
      init_unk = &D_0013E550;
      D_0013D290.unkF4 = 1;
      e8_scale = ((s32) (D_0015EDF0 * 8)) / 10;
      D_0013E550.unk4C = *((s32 *) 0x15EDEC);
      e7_scale = ((s32) (D_0015EDF0 * 7)) / 10;
      D_0013E550.unk48 = e8_scale;
      D_0013E550.unk50 = e8_scale;
      D_0013E550.unk54 = ((s32) (D_0015EDF0 * 7)) / 10;
      e7_scale = ((s32) (D_0015EDF0 * 7)) / 10;
      D_0013E550.unk5C = D_0015EDF0;
      D_0013E550.unk58 = e7_scale;
      asm volatile("" : : "r"(init_unk));
      InitializeGlobalStateEntry(D_0015ED84, init_unk, D_0015EDF0, 0xA, 0xA, e8_scale);
      *((s16 *) 0x13E05A) = 0;
    }
    else
    {
      return 0;
    }
  }
  if (D_0013C940.unk1C4 & 0xD00)
  {
    if (D_001D5BF0.unk124 == 0)
    {
      return 1;
    }
  }
  if (D_0013C940.unk1C4 & 0x10)
  {
    mobj_unk38 = D_001D5BF0.unk4->unk38;
    if (mobj_unk38 != 0)
    {
      D_001D5BF0.unk8 = mobj_unk38;
    }
    else
      if (D_001D5BF0.unk124 == 0)
    {
      return -1;
    }
  }
  if ((D_0015EEB0 != 0x10) && (D_0015EEB0 != 1))
  {
    D_001D5BF0.unk8 = D_001D5BF0.unk4->unk38;
    return 0;
  }
  if (((D_0013D290.unkD4 < 3) && (D_0013D290.unkDC < 0)) && (D_0013D290.unk8 == 2))
  {
    if (arg0->unk30 & 1)
    {
      flags = D_0013C940.unk1B4;
    }
    else
    {
      flags = D_0013C940.unk1C4;
    }
    arg0->unk40 = (s32) D_0015EE34;
    if ((flags & 0x1000) && (D_0015EE34 != 0))
    {
      arg0->unk40 = (s32) (D_0015EE34 - 1);
    }
    sub_unk40 = arg0->unk40;
    if ((flags & 0x4000) && (sub_unk40 < 4))
    {
      arg0->unk40 = (s32) (sub_unk40 + 1);
    }
    D_0015EE34 = arg0->unk40;
    if (((flags & 0x40) && (D_0013D290.unk8 == 2)) && ((*((s32 *) ((((u8 *) (&D_0013D290)) + (arg0->unk40 * 0x1C)) + 0x20))) >= 0))
    {
      func_0022DA68(0, 0x11, arg0->unk14);
      D_0013D290.unkC0 = 0;
      D_0013D290.unk14 = (s32) arg0->unk40;
      if (D_0013D290.unkDC < 0)
      {
        D_0013D290.unkE0 = 0;
        D_0013D290.unkDC = 0xD;
      }
      D_001D5BF0.unk128 = 1;
      D_001D5BF0.unk12C = 0x4FB6;
    }
    if (arg0->unk40 != temp_19_15)
    {
      func_0022DA68(1, 0x11, arg0->unk14);
    }
  }
  return 0;
}
#endif /* NON_MATCHING */
