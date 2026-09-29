
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
struct PadState
{
  u8 pad0[0x1A0];
  s64 unk1A0;
  u8 pad1A4[0x18];
  u32 held;
  u32 pressed;
};
struct MenuCur
{
  u8 pad0[0x38];
  s32 unk38;
};
struct MenuSys
{
  u8 pad0[4];
  struct MenuCur *cur;
  s32 unk8;
  u8 padC[0xD0];
  s32 unkDC;
  u8 padE0[0x44];
  s32 unk124;
};
struct Menu
{
  u8 pad0[0x34];
  s32 str;
  u8 pad38[4];
  s32 unk3C;
  s32 unk40;
  u8 pad44[0xC];
  u32 unk50;
  s32 unk54;
};
extern struct PadState D_0013C940;
extern struct MenuSys D_001D5BF0;
extern s32 D_001D2AF4[];
extern u8 D_001D5098[];
extern u8 D_001D5148[];
extern u8 D_001D5208[];
extern u8 D_001D5298[];
extern u8 D_001D52B0[];
extern s32 FUN_001f96f8(s32);
s32 FUN_002223f0(struct Menu *arg0)
{
  int flag;
  s32 n;

  flag = (D_0013C940.unk1A0 & 0xF) == 0xF;
  if ((D_001D5BF0.unkDC == 0) || (flag && (((s32) D_0013C940.unk1A0) & 0x10)))
  {
    if ((D_0013C940.pressed & 0xD00) && (D_001D5BF0.unk124 == 0))
    {
      return 1;
    }
    if (D_0013C940.pressed & 0x10)
    {
      if (D_001D5BF0.cur->unk38 != 0)
      {
        D_001D5BF0.unk8 = D_001D5BF0.cur->unk38;
      }
      else
        if (D_001D5BF0.unk124 == 0)
      {
        return -1;
      }
    }
  }
  n = arg0->unk40;
  arg0->unk40 = n - 1;
  if (flag)
  {
    arg0->unk40 = n - 2;
  }
  if (arg0->unk40 < 0)
  {
    arg0->unk40 = 0;
  }
  switch (arg0->unk50)
  {
    case 0:
      arg0->unk50 = arg0->unk50 + 1;
      arg0->unk40 = FUN_001f96f8(0xB4);
      arg0->unk3C = 0;
      D_001D2AF4[0] = 0x50A9;
      arg0->str = (s32) D_001D5098;
      break;

    case 1:

    case 5:

    case 8:

    case 12:

    case 16:
      if (arg0->unk40 == 0)
    {
      arg0->unk54 = 0;
      arg0->unk50 = *(volatile u32 *) (&arg0->unk50) + 1;
    }
      break;

    case 2:

    case 6:

    case 9:

    case 13:

    case 17:
      n = arg0->unk3C;
      arg0->unk3C = n + 0xA;
      if (flag)
    {
      arg0->unk3C = n + 0x14;
    }
      if (arg0->unk54 != 0)
    {
      arg0->unk40 = FUN_001f96f8(0xB4);
      arg0->unk50 = arg0->unk50 + 1;
    }
      break;

    case 4:
      if (arg0->unk40 == 0)
    {
      arg0->unk50 = 5;
      arg0->unk40 = FUN_001f96f8(0xB4);
      arg0->unk3C = 0;
      D_001D2AF4[0] = 0x50D6;
      arg0->str = (s32) D_001D5148;
    }
      break;

    case 3:
      if (arg0->unk40 == 0)
    {
      D_001D2AF4[0] = 0x50D4;
      arg0->str = 0x50D5;
      arg0->unk40 = FUN_001f96f8(0xF0);
      arg0->unk3C = 0;
      arg0->unk50 = arg0->unk50 + 1;
    }
      break;

    case 7:
      if (arg0->unk40 == 0)
    {
      D_001D2AF4[0] = 0x5106;
      arg0->str = (s32) D_001D5208;
      arg0->unk40 = FUN_001f96f8(0xF0);
      arg0->unk3C = 0;
      arg0->unk50 = arg0->unk50 + 1;
    }
      break;

    case 10:
      if (arg0->unk40 == 0)
    {
      D_001D2AF4[0] = 0x5136;
      arg0->str = 0x5137;
      arg0->unk40 = FUN_001f96f8(0xF0);
      arg0->unk3C = 0;
      arg0->unk50 = arg0->unk50 + 1;
    }
      break;

    case 11:
      if (arg0->unk40 == 0)
    {
      arg0->unk40 = FUN_001f96f8(0xB4);
      arg0->unk3C = 0;
      D_001D2AF4[0] = 0x5138;
      arg0->str = (s32) D_001D5298;
      arg0->unk50 = 0xC;
    }
      break;

    case 14:
      if (arg0->unk40 == 0)
    {
      D_001D2AF4[0] = 0;
      arg0->str = 0x5143;
      arg0->unk40 = FUN_001f96f8(0xF0);
      arg0->unk3C = 0;
      arg0->unk50 = arg0->unk50 + 1;
    }
      break;

    case 15:
      if (arg0->unk40 == 0)
    {
      arg0->unk40 = FUN_001f96f8(0xB4);
      arg0->unk3C = 0;
      D_001D2AF4[0] = 0x5144;
      arg0->str = (s32) D_001D52B0;
      arg0->unk50 = 0x10;
    }
      break;

    case 18:
      if (arg0->unk40 == 0)
    {
      D_001D2AF4[0] = 0;
      arg0->str = 0x5175;
      arg0->unk50 = arg0->unk50 + 1;
      arg0->unk40 = FUN_001f96f8(0x12C);
    }
      break;

    case 19:
      if (arg0->unk40 == 0)
    {
      if (D_001D5BF0.unkDC != 0)
      {
        return 1;
      }
    }
      break;

  }

  return 0;
}

extern __typeof__(FUN_002223f0) func_002223F0 __attribute__((alias("FUN_002223f0")));
