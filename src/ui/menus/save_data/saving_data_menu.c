
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
typedef struct __attribute__((packed)) { s64 v; } Unaligned64;
extern struct M2c_D_0013C940 D_0013C940;
extern Unaligned64 D_0015EE98[1];
extern struct M2c_D_0013D290 D_0013D290;
extern s32 D_0015ED84;
extern s32 D_0015ED98;
extern s32 D_0015EE20;
extern s32 D_0015EE24;
extern s32 D_0015EE34;
extern s32 D_0015EEB0;
extern s32 D_0015EEB4;
extern u8 D_001D2578[];
extern s32 D_001D2640[];
extern struct M2c_D_001D5BF0 D_001D5BF0;
extern void func_001FBAB8();
extern s32 FUN_002269c0();
extern s32 func_0022DA68();
s32 saving_data_menu(struct M2c_arg0 *arg0) __asm__("FUN_00222f88");

s32 saving_data_menu(struct M2c_arg0 *arg0)
{
  u8 *new_var;
  s32 *temp_2_126;
  s32 temp_11_47;
  s32 temp_17_29;
  s32 temp_4_184;
  s32 temp_5_190;
  s32 var_3_168;
  if (D_001D5BF0.unk4->unk40 == arg0)
  {
    if (((arg0->unk4C == 0) && (D_001D5BF0.unkD0 == D_001D2578)) && (D_001D5BF0.unkD0->unk84 != 0))
    {
      arg0->unk4C = 1;
    }
    temp_17_29 = arg0->unk4C;
    if (temp_17_29 == 1)
    {
      FUN_002269c0(arg0->unk48, arg0->unk40);
      D_001D5BF0.unk128 = temp_17_29;
      D_001D5BF0.unk12C = 0x4FB5;
    }
    arg0->unk4C = 2;
    temp_11_47 = arg0->unk40;
    if (D_001D5BF0.unk128 != 0)
    {
      if ((D_0013D290.unkD4 < 3) && (D_0013D290.unkDC < 0))
      {
        D_001D5BF0.unk128 = 0;
        if (D_0013D290.unkE4 != 0)
        {
          D_0015EEB4 |= 0x80;
          func_001FBAB8(3, D_001D5BF0.unk4);
          goto block_46;
        }
        *((s32 *) ((((u8 *) (&D_0013D290)) + (D_0013D290.unk14 * 0x1C)) + 0x24)) = (s32) D_0015ED98;
        *((s32 *) ((((u8 *) (&D_0013D290)) + (D_0013D290.unk14 * 0x1C)) + 0x20)) = (s32) D_0015ED84;
        *((s32 *) ((((u8 *) (&D_0013D290)) + (D_0013D290.unk14 * 0x1C)) + 0x2C)) = (s32) D_0015EE24;
        *((Unaligned64 *) ((((u8 *) (&D_0013D290)) + (D_0013D290.unk14 * 0x1C)) + 0x30)) = D_0015EE98[0];
        *((s32 *) ((((u8 *) (&D_0013D290)) + (D_0013D290.unk14 * 0x1C)) + 0x28)) = (s32) D_0015EE20;
        goto block_13;
      }
      return 0;
    }
    block_13:
    if (0xD00 & D_0013C940.unk1C4)
    {
      if (D_001D5BF0.unk124 == 0)
      {
        return 1;
      }
      goto block_17;
    }

    block_17:
    if (D_0013C940.unk1C4 & 0x10)
    {
      temp_2_126 = D_001D5BF0.unk4->unk38;
      if (temp_2_126 != 0)
      {
        D_001D5BF0.unk8 = temp_2_126;
        goto block_23;
      }
      if (D_001D5BF0.unk124 == 0)
      {
        return -1;
      }
      goto block_24;
    }

    block_23:
    block_24:
    if ((D_0015EEB0 != 0x10) && (D_0015EEB0 != 1))
    {
      D_001D5BF0.unk8 = (void *) D_001D5BF0.unk4->unk38;
      return 0;
    }


    if (((D_0013D290.unkD4 < 3) && (D_0013D290.unkDC < 0)) && (D_0013D290.unk8 == 2))
    {
      if (arg0->unk30 & 1)
      {
        var_3_168 = D_0013C940.unk1B4;
      }
      else
      {
        var_3_168 = D_0013C940.unk1A4;
      }
      arg0->unk40 = (s32) D_0015EE34;
      if ((var_3_168 & 0x1000) && (D_0015EE34 != 0))
      {
        arg0->unk40 = (s32) (D_0015EE34 - 1);
      }
      if (var_3_168 & 0x4000)
      {
        temp_4_184 = arg0->unk40;
        if (temp_4_184 < 4)
        {
          arg0->unk40 = (s32) (temp_4_184 + 1);
        }
      }
      temp_5_190 = arg0->unk40;
      D_0015EE34 = temp_5_190;
      if ((var_3_168 & 0x40) && (D_0013D290.unk8 == 2))
      {
        new_var = (((u8 *) (&D_0013D290)) + (temp_5_190 * 0x1C)) + 0x20;
        if ((*((s32 *) new_var)) != (-1))
        {
          D_001D5BF0.unkD4 = 0;
          D_001D5BF0.unk8 = (new_var = D_001D2578);
          D_001D2640[0] = arg0->unk40;
        }
        else
        {
          arg0->unk4C = 1;
        }
      }
      if (arg0->unk40 != temp_11_47)
      {
        func_0022DA68(1, 0x11, arg0->unk14);
      }
      block_46:
      ;

      ;
    }
    return 0;
  }
  return 0;
}

extern __typeof__(saving_data_menu) func_00222F88 __attribute__((alias("FUN_00222f88")));
