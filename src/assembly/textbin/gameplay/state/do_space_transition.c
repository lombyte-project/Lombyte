#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/gameplay/state/do_space_transition/FUN_00231ff0.s", FUN_00231ff0);
#else
#include "rnc/gameplay_state_do_space_transition_types.h"
extern struct M2c_D_0013D290 D_0013D290;
extern struct M2c_D_0013DD40 D_0013DD40;
extern struct M2c_D_0013DD58 D_0013DD58;
extern s32 D_0013DD5C;
extern s32 D_0013DD5F[];
extern s32 D_0013DD65;
extern s32 D_0013DD66;
extern s32 D_0013DD68;
extern struct M2c_D_0013E030 D_0013E030;
extern struct M2c_D_001516D0 D_001516D0;
extern s32 D_0015ED5C;
extern s32 D_0015ED84;
extern s32 D_0015ED88;
extern s16 D_0015EE48;
extern s16 D_0015EE4A;
extern s32 D_0015F438;
extern s32 D_0015F600;
extern s32 D_0015F604;
extern s32 D_0015F618[];
extern struct M2c_D_0015F634 *D_0015F634;
extern struct M2c_D_0018CD00 D_0018CD00;
extern struct M2c_D_00194100 D_00194100;
extern u8 D_001E8988[];
extern void DebugPrint();
extern void FlushCache();
extern void PackDmaTag(u64, u64, u64);
extern void func_0012DC80();
extern void func_0012E1A8(void);
extern void func_0012E1D8(s32);
extern void func_0012EB00();
extern s32 func_0012EE08(s32);
extern s32 func_0012EF28(s32);
extern void func_0012EF68(s32, s32, s32, s32, s32);
extern void func_0012F368();
extern void func_001F4A58();
extern s32 func_001F96F8();
extern void func_001FB2D0();
extern void func_001FB3D0();
extern void func_001FB6E0();
extern void func_002043B0();
extern s32 func_00204428();
extern void func_00208840();
extern void func_002093D8();
extern void func_00215EE8(void);
extern void func_00217A10(void);
extern void func_00226E08(void);
extern void func_0022DCD0();
extern void func_0022F778();
extern void func_00230EE8();
extern void func_00230F60();
extern void func_00231608();
extern void func_00231BD8();
extern void func_00233630(void);
extern void func_002336A0();
extern void func_002337B0();
extern void func_00233D90(void);
extern s32 sceCdSync(s32);
extern void sceGsSyncV();
inline int inline_fn()
{
  return D_0013DD66 == 2;
}

inline int inline_fn2(u8 arg0)
{
  return arg0 == 0;
}

void do_space_transition(void) __asm__("FUN_00231ff0");

void do_space_transition(void)
{
  u8 *new_var7;
  int new_var6;
  s32 temp_16_24;
  s32 temp_16_34;
  s32 temp_2_465;
  s32 temp_3_266;
  s32 new_var5;
  s32 new_var;
  int temp_3_343;
  s32 temp_5_26;
  u8 *new_var3;
  s32 var_16_421;
  int new_var4;
  s32 var_5_398;
  temp_16_24 = D_0015ED88 - 1;
  temp_5_26 = D_00194100.unk10;
  temp_5_26 = temp_5_26 | 0x80000000;
  D_00194100.unk10 = temp_5_26;
  *((s32 *) 0x15F604) = 6;
  temp_16_34 = (temp_16_24 <= (-1)) ? (0) : (temp_16_24);
  D_0013E030.unk26 = 0;
  if (D_0013DD40.unk8 != 0)
  {
    goto block_2;
  }
  if (D_0015F600 < 8)
  {
    goto block_3;
  }
  block_2:
  D_0013E030.unk26 = 1;

  block_3:
  if (D_0013DD40.unkE != 0)
  {
    goto block_5;
  }

  if (D_0015F600 < 0xE)
  {
    goto block_6;
  }
  block_5:
  D_0013E030.unk26 = 2;

  block_6:
  FlushCache(0, temp_5_26, *(new_var7 = &D_0013DD40.unk8), &D_00194100, &D_0013E030, &D_0013E030);

  func_0012EF68(2, 0, 0, 0, 0);
  func_0012EB00();
  func_0012DC80();
  func_0022DCD0();
  func_00215EE8();
  D_001516D0.unkB = 1;
  if (D_0015F634 == 0)
  {
    goto block_8;
  }
  func_0012E1D8(D_0015F634->unk1C);
  func_0012E1A8();
  DebugPrint(D_001E8988, D_0015F634->unk1C);
  block_8:
  D_0015ED5C = 0;

  func_0012EF28(0);
  func_0012EE08(0);
  D_0018CD00.unk238 = 0x10;
  D_0018CD00.unk22C = 128.0f;
  D_0018CD00.unk21C = 524288.0f;
  D_0018CD00.unk228 = 255.0f;
  D_0018CD00.unk218 = (D_0018CD00.unk230 = 0);
  D_0018CD00.unk234 = 0;
  PackDmaTag(0, 0, 0);
  new_var4 = D_0015F600 >= ((0, 0));
  if (new_var4)
  {
    goto block_15;
  }
  if (D_0013D290.unkD4 >= 3)
  {
    goto loop_12;
  }
  if (D_0013D290.unkDC < 0)
  {
    goto block_14;
  }
  loop_12:
  func_002093D8();

  func_00208840();
  if (D_0013D290.unkD4 >= 3)
  {
    goto loop_12;
  }
  temp_3_343 = D_0013D290.unkDC;
  if (temp_3_343 >= 0)
  {
    goto loop_12;
  }
  block_14:
  func_001F4A58(func_001F96F8(6));

  D_0015ED84 = D_0015F600;
  func_002043B0();
  sceCdSync(0);
  D_001516D0.unkB = 0;
  func_00233D90();
  return;
  block_15:
  if (D_0015F600 != 0)
  {
    goto block_18;
  }

  if (D_0013DD58.unk0 != 0)
  {
    goto block_18;
  }
  func_001F4A58(func_001F96F8(6));
  func_00231BD8(temp_16_34, 0, 1, func_001F96F8(0xF0), 0);
  func_00231608(0);
  func_00231BD8(temp_16_34, 2, 2, func_001F96F8(0xB4), 0);
  func_00231608(1);
  D_0015ED84 = D_0015F600;
  func_00231BD8(temp_16_34, 3, 4, func_001F96F8(0xF0), 1);
  func_00231608(2);
  goto block_64;
  block_18:
  if (D_0015ED84 != 0)
  {
    goto block_22;
  }

  if (D_0015F600 != 1)
  {
    goto block_23;
  }
  new_var3 = (u8 *) ((u8 *) (&D_0013DD58));
  if (new_var3[1] != 0)
  {
    goto block_22;
  }
  func_001F4A58(func_001F96F8(6));
  func_00231BD8(temp_16_34, 5, 6, func_001F96F8(0xF0), 0);
  func_00231608(3);
  func_00231608(4);
  func_00231BD8(temp_16_34, 7, 7, func_001F96F8(0xB4), 0);
  new_var = D_0013DD5C;
  func_00231608(5);
  *((u8 *) (new_var3 + D_0015F600)) = 2;
  D_0015ED84 = D_0015F600;
  func_00231BD8(temp_16_34, 8, 8, func_001F96F8(0xF0, D_0015F600), 1);
  goto block_64;
  block_22:
  block_23:
  temp_3_266 = D_0015ED84;


  if (D_0015F600 != 4)
  {
    goto block_26;
  }
  if (new_var != 0)
  {
    goto block_26;
  }
  func_001F4A58(func_001F96F8(0xC));
  func_00231BD8(temp_16_34, 9, 0xA, func_001F96F8(0xF0), 0);
  func_00231608(6);
  block_26:
  if (D_0015F600 != 7)
  {
    goto block_31;
  }
  if (D_0013DD5F[0] == 2)
  {
    goto block_30;
  }
  if (inline_fn2(D_0013DD40.unk8))
  {
    goto block_31;
  }
  func_001F4A58(func_001F96F8(0xC) & 0xFFu);
  func_00231BD8(temp_16_34, 0xB, 0xB, func_001F96F8(0xF0), 0);
  func_00231608(7);
  block_30:
  block_31:
  temp_3_343 = 0xD;
  if (D_0015F600 != 0xD)
  {
    goto block_34;
  }
  if (*((u8 *) 0x13DD65) != 0)
  {
    goto block_34;
  }
  func_001F4A58(func_001F96F8(0xC));
  func_00231BD8(temp_16_34, 0xC, temp_3_343, func_001F96F8(0xF0), 0);
  func_00231608(8);
  block_34:
  ;

  ;
  ;
  ;
  ;
  ;
  ;
  if (D_0015ED84 != 0xE)
  {
    goto block_39;
  }
  if (inline_fn())
  {
    goto block_38;
  }
  if (inline_fn2(D_0013DD40.unkF))
  {
    goto block_39;
  }
  func_001F4A58(func_001F96F8(0xC));
  func_00231BD8(temp_16_34, 0xE, 0xE, func_001F96F8(0xF0), 0);
  func_00231608(9);
  block_38:
  block_39:
  if (D_0015F600 == 0x10)
  {
    goto block_41;
  }


  goto block_46;
  block_41:
  if (inline_fn2(*((s32 *) 0x13DD68)))
  {
    goto block_44;
  }

  goto block_46;
  block_44:
  func_001F4A58(func_001F96F8(0xC));

  func_00231BD8(temp_16_34, 0xF, 0x10, func_001F96F8(0xF0), 0);
  func_00231608(0xA);
  block_46:
  if (((u32) D_0015ED84) >= 0x13U)
  {
    goto block_53;
  }

  var_5_398 = 1;
  new_var6 = 0;
  if (((u32) D_0015ED84) != 7)
  {
    goto block_49;
  }
  var_5_398 = (inline_fn2(D_0013DD40.unk8)) ? (0) : (1);
  block_49:
  if (((u32) D_0015ED84) != 0xE)
  {
    goto block_51;
  }

  var_5_398 = (inline_fn2(D_0013DD40.unkF)) ? (0) : (var_5_398);
  block_51:
  if (var_5_398 == 0)
  {
    goto block_53;
  }

  *(((u8 *) ((u8 *) (&D_0013DD58))) + D_0015ED84) = 2;
  block_53:
  D_0015EE4A = 1;

  var_16_421 = new_var6;
  D_0015ED84 = D_0015F600;
  D_0015EE48 = 0;
  func_00230F60((char) ((u32) D_0015ED84));
  func_0012F368(D_0015ED84);
  new_var5 = D_0015F618[0];
  if (new_var5 != 0)
  {
    goto block_58;
    do
    {
    }
    while (0);
  }
  loop_55:
  func_002336A0();

  func_00233630();
  func_001FB3D0();
  func_001FB6E0();
  func_001FB2D0();
  func_00217A10();
  func_0022F778();
  func_00230EE8();
  func_002093D8();
  func_00208840();
  func_002337B0(1);
  sceGsSyncV(0);
  D_0015F438 += 1;
  func_00226E08();
  temp_2_465 = new_var5;
  if (var_16_421 != 0)
  {
    goto block_57;
  }
  var_16_421 = func_00204428();
  block_57:
  if (temp_2_465 == 0)
  {
    goto loop_55;
  }

  block_58:
  if (var_16_421 != 0)
  {
    goto loop_62;
  }

  loop_59:
  FlushCache(0);

  sceGsSyncV(0);
  func_002093D8();
  func_00208840();
  func_00226E08();
  if (func_00204428() == 0)
  {
    goto loop_59;
  }
  goto loop_62;
  block_61:
  FlushCache(0);

  sceGsSyncV(0);
  func_002093D8();
  func_00208840();
  func_00226E08();
  loop_62:
  if (D_0013D290.unkD4 != 2)
  {
    goto block_61;
  }

  if (D_0013D290.unkDC >= 0)
  {
    goto block_61;
  }
  block_64:
  sceCdSync(0);

  D_001516D0.unkB = 0;
  func_00233D90();
  return;
}

__attribute__((alias("FUN_00231ff0"))) extern void func_00231FF0(void);
#endif /* NON_MATCHING */
