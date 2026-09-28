typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
struct M2c_D_0013D290
{
  u8 pad_0[0x8];
  s32 unk8;
  u8 pad_C[0xC8];
  s32 unkD4;
  u8 pad_D8[0x4];
  s32 unkDC;
};
struct M2c_D_001D5BF4
{
  u8 pad_0[0x40];
  struct M2c_D_001D5BF4_unk40 *unk40;
};
struct M2c_D_001D5BF4_unk40
{
  u8 pad_0[0x40];
  s32 unk40;
};
struct M2c_arg0
{
  u8 pad_0[0x20];
  s32 unk20;
  s32 unk24;
};
extern struct M2c_D_0013D290 D_0013D290;
extern struct M2c_D_001D5BF4 *D_001D5BF4[];
extern u8 D_001DDD40[];
extern void func_001F4280(s32);
extern s32 func_001F4398();
extern s32 func_001F6AF0(s32, s32, u64, s32, s32);
extern s32 func_001FDD10();
s32 FUN_00222d98(struct M2c_arg0 *arg0)
{
  s32 temp_16_44;
  s32 temp_16_80;
  s32 temp_17_21;
  s32 temp_17_42;
  s32 temp_18_57;
  s32 temp_16_62;
  u32 temp_16_34;
  u32 temp_16_56;
  u32 temp_16_74;
  u32 temp_17_36;
  struct M2c_D_0013D290 *base;
  base = &D_0013D290;
  temp_17_21 = *((s32 *) ((((u8 *) base) - (-(D_001D5BF4[0]->unk40->unk40 * 0x1C))) + 0x20));
  func_001F4280(0);
  if (((base->unkD4 < 3) && (base->unkDC < 0)) && (base->unk8 == 2))
  {
    if (temp_17_21 == (-1))
    {
      temp_16_34 = arg0->unk24;
      temp_17_36 = arg0->unk20;
      temp_17_42 = ((s32) (temp_17_36 + (((u32) temp_17_36) >> 0x1F))) >> 1;
      temp_16_44 = (((s32) (temp_16_34 + (((u32) temp_16_34) >> 0x1F))) >> 1) - 8;
      func_001F6AF0(temp_17_42, temp_16_44, 0x80FFA888, func_001FDD10(0x5217), -1);
    }
    else
    {
      temp_16_56 = arg0->unk20;
      temp_18_57 = temp_17_21 * 0xC;
      temp_16_62 = ((s32) (temp_16_56 + ((((u32) temp_16_56) >> 24) >> 7))) >> 1;
      func_001F6AF0(temp_16_62, 4, 0x80FFA888, func_001FDD10(*((s32 *) ((void *) (temp_18_57 + D_001DDD40)))), -1);
      temp_16_74 = arg0->unk20;
      if (1)
      {
        temp_16_80 = ((s32) (temp_16_74 + (((u32) temp_16_74) >> 0x1F))) >> 1;
        func_001F6AF0(temp_16_80, 0x14, 0x80FFA888, func_001FDD10(*((s32 *) (((u8 *) (D_001DDD40 + temp_18_57)) + 0x4))), -1);
      }
    }
  }
  func_001F4398();
  return 2;
}
