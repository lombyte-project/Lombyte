#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/gameplay/state/transition_load_wad/FUN_001ea830.s", FUN_001ea830);
#else
#include "rnc/gameplay_state_transition_load_wad_types.h"
extern struct M2c_D_00137B80 D_00137B80;
extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern s32 D_0015EE8C[];
extern s32 D_0015EF48;
extern s32 D_0015EF58;
extern s32 D_0015EF60;
extern s32 D_0015EF64;
extern s32 D_0015F460;
extern s32 D_0015F6A0;
extern s32 D_0015FF00;
extern s32 D_0015FF08;
extern s32 D_001603CC;
extern s32 D_001603EC;
extern s32 D_00160E94[];
extern s32 D_00160F0C;
extern s32 D_00160F4C;
extern s32 D_00160F64;
extern u8 D_001861E0[];
extern u8 D_00186310[];
extern struct M2c_D_0018CB20 D_0018CB20;
extern struct M2c_D_001940C0 D_001940C0;
extern u8 D_00194180[];
extern struct M2c_D_001996D0 D_001996D0;
extern struct M2c_D_0019E6C0 D_0019E6C0;
extern u8 D_001B3200[];
extern struct M2c_D_001B3AC0 D_001B3AC0;
extern u8 D_001B5980[];
extern u8 D_001B6180[];
extern u8 D_001B6880[];
extern u8 D_001D84B0[];
extern u8 D_001E0900[];
extern u8 D_001E2600[];
extern void CalculateDmaTransferAddress();
extern void FillTransferWords();
extern void FlushCache();
extern void QueueDmaTransfer();
extern void func_00120558();
extern s32 func_001E9B10();
extern void func_001F2C60();
extern void func_001F2D98();
extern s32 func_001F97A0();
extern void func_002015D8();
extern void func_002026C8();
extern void func_00202800();
extern void func_002028E0();
extern void func_00203120();
extern void func_00203640();
extern void func_00203730();
extern void func_00203B08();
extern void func_002040E0();
extern void func_002049F0();
extern s32 func_0020B618();
extern s32 func_00216788(s32, s32, s32);
extern s32 func_00216828(s32, s32, s32);
extern s16 func_002168A8(s32);
extern void func_002335D0();
extern void sceGsExecLoadImage();
extern void sceGsSetDefLoadImage();
void transition_load_wad(void) __asm__("FUN_001ea830");

void transition_load_wad(void)
{
  u8 sp_slot[0x100];
  struct M2c_var_16_268 *var_16_268;
  struct M2c_var_5_385 *var_5_385;
  s32 temp_22_82;
  s32 temp_2_142;
  s32 temp_2_155;
  s32 temp_2_168;
  s32 temp_2_181;
  s32 temp_2_194;
  s32 temp_2_207;
  s32 temp_2_220;
  volatile unsigned int new_var2;
  s32 temp_2_250;
  s32 temp_2_379;
  s32 temp_2_390;
  s32 temp_3_129;
  s32 temp_3_141;
  s32 temp_3_167;
  s32 temp_3_193;
  s32 temp_3_219;
  s32 temp_5_356;
  s32 temp_7_275;
  s32 var_18_154;
  s32 var_18_180;
  s32 var_18_206;
  unsigned long new_var;
  s32 var_18_232;
  s32 var_18_265;
  s32 var_18_286;
  s32 var_18_411;
  s32 var_19_21;
  s32 var_4_269;
  s32 var_5_435;
  s32 var_7_388;
  s64 temp_2_359;
  s64 temp_7_353;
  void **temp_4_466;
  void **var_18_135;
  void **var_19_161;
  void **var_19_187;
  void **var_19_213;
  void **var_3_440;
  void **var_6_389;
  struct M2c_temp_17_85 *temp_17_85;
  s32 *temp_20_126;
  s32 *temp_21_115;
  s32 *temp_6_272;
  struct M2c_var_16_134 *var_16_134;
  struct M2c_var_16_160 *var_16_160;
  struct M2c_var_16_186 *var_16_186;
  struct M2c_var_16_212 *var_16_212;
  struct M2c_var_16_242 *var_16_242;
  struct M2c_var_16_289 *var_16_289;
  D_0015EF58 = 0;
  ;
  CalculateDmaTransferAddress();
  func_002015D8();
  D_00160F0C = 0x100000;
  D_0015EE74 = 0x2C0000;
  D_0015EE8C[0] = 0x2C0000;
  D_0015EE78 = 0x2C0000;
  FillTransferWords(D_00194180, 0x87654321, 0x10);
  FillTransferWords(&D_001B3AC0, -1, 0x800);
  FillTransferWords(D_001B6880, -1, 0xE00);
  FillTransferWords(D_001B6180, 0, 0xE0);
  func_001F2C60();
  func_001F2D98();
  func_002335D0();
  func_00216788(D_001940C0.unk14 + 0x01000000, D_00137B80.unk14E8, D_00137B80.unk14EC);
  func_002168A8(1);
  FlushCache(0);
  temp_22_82 = func_0020B618(D_001940C0.unk14 + 0x01000000, D_001940C0.unk14);
  FlushCache(0);
  temp_17_85 = D_001940C0.unk14;
  new_var = 0xB800;
  func_00203120(((u8 *) temp_17_85) + ((u32) temp_17_85->unk0), (*temp_17_85).unk8, ((u8 *) temp_17_85) + ((u32) temp_17_85->unkC));
  temp_21_115 = ((u8 *) temp_17_85) + ((u32) temp_17_85->unk4);
  D_0019E6C0.unk0 = (s64) ((((((s32) (D_0015EE8C[0] + temp_17_85->unk70)) >> 8) | 0x1D308000) | ((((s64) (((s32) (D_0015EE8C[0] + temp_17_85->unk74)) >> 8)) << 0x25) | (new_var << 0x13))) | ((-1) << 0x3F));
  D_0019E6C0.unk10 = (s64) ((((0x8000 << 0x14) | 0x8000) << 0x13) | 0x4000);
  temp_20_126 = temp_21_115 + temp_17_85->unk60;
  D_0019E6C0.unk8 = (s64) ((0xFFA0 << 0x20) | 0xE0);
  temp_3_129 = temp_17_85->unk30;
  D_00160E94[0] = temp_3_129;
  if (temp_3_129 > 0)
  {
    var_16_134 = ((u8 *) temp_17_85) + ((u32) temp_17_85->unk34);
    var_18_135 = D_001E0900;
    do
    {
      var_19_21 += 1;
      temp_3_141 = var_16_134->unk0;
      temp_2_142 = func_001F97A0(var_16_134->unk4) << 0x1C;
      var_16_134 += 0x10;
      *var_18_135 = (temp_20_126 + temp_3_141) + temp_2_142;
      var_18_135 += 4;
    }
    while (0 < D_00160E94[0]);
  }
  var_18_154 = 0;
  new_var2 = (u32) temp_17_85->unk3C;
  temp_2_155 = temp_17_85->unk38;
  D_0015FF08 = temp_2_155;
  if (temp_2_155 > 0)
  {
    var_16_160 = ((u8 *) temp_17_85) + new_var2;
    var_19_161 = D_001B5980;
    do
    {
      var_18_154 += 1;
      temp_3_167 = var_16_160->unk0;
      temp_2_168 = func_001F97A0(var_16_160->unk4) << 0x1C;
      var_16_160 += 0x10;
      *var_19_161 = (temp_20_126 + temp_3_167) + temp_2_168;
      var_19_161 += 4;
    }
    while (var_18_154 < D_0015FF08);
  }
  var_18_180 = 0;
  temp_2_181 = temp_17_85->unk40;
  D_00160F64 = temp_2_181;
  if (temp_2_181 > 0)
  {
    var_16_186 = ((u8 *) temp_17_85) + ((u32) temp_17_85->unk44);
    var_19_187 = D_001E2600;
    do
    {
      var_18_180 += 1;
      temp_3_193 = (new_var = var_16_186->unk0);
      temp_2_194 = func_001F97A0(var_16_186->unk4) << 0x1C;
      var_16_186 += 0x10;
      *var_19_187 = (temp_20_126 + temp_3_193) + temp_2_194;
      var_19_187 += 4;
    }
    while (var_18_180 < D_00160F64);
  }
  var_18_206 = 0;
  temp_2_207 = temp_17_85->unk48;
  D_001603EC = temp_2_207;
  if (temp_2_207 > 0)
  {
    var_16_212 = ((u8 *) temp_17_85) + ((u32) temp_17_85->unk4C);
    var_19_213 = D_001D84B0;
    do
    {
      var_18_206 += 1;
      temp_3_219 = var_16_212->unk0;
      temp_2_220 = func_001F97A0(var_16_212->unk4) << 0x1C;
      var_16_212 += 0x10;
      *var_19_213 = (temp_20_126 + temp_3_219) + temp_2_220;
      var_19_213 += 4;
    }
    while (var_18_206 < D_001603EC);
  }
  var_18_232 = 0;
  func_002040E0(temp_21_115 + temp_17_85->unk10, ((u8 *) temp_17_85) + ((u32) temp_17_85->unk34));
  func_002028E0(temp_21_115 + temp_17_85->unk14);
  var_16_242 = ((u8 *) temp_17_85) + ((u32) temp_17_85->unk1C);
  D_0015FF00 = 0;
  D_00160F4C = 0;
  D_001603CC = 0;
  if (temp_17_85->unk18 > 0)
  {
    do
    {
      temp_2_250 = var_16_242->unk0;
      var_18_232 += 1;
      func_00203640((temp_2_250 == 0) ? (0) : (temp_21_115 + temp_2_250), ((u8 *) temp_17_85) + ((u32) temp_17_85->unk3C), ((u8 *) var_16_242) + 0x10, var_16_242->unk4);
      var_16_242 += 0x20;
    }
    while (var_18_232 < temp_17_85->unk18);
  }
  var_18_265 = 0;
  var_16_268 = ((u8 *) temp_17_85) + ((u32) temp_17_85->unk24);
  if (temp_17_85->unk20 > 0)
  {
    var_4_269 = *((s32 *) var_16_268);
    loop_16:
    temp_6_272 = ((u8 *) var_16_268) + 0x10;

    var_18_265 += 1;
    temp_7_275 = var_16_268->unk4;
    var_16_268 += 0x20;
    func_00203730(temp_21_115 + var_4_269, ((u8 *) temp_17_85) + ((u32) temp_17_85->unk44), temp_6_272, temp_7_275);
    if (var_18_265 < temp_17_85->unk20)
    {
      var_4_269 = *((s32 *) var_16_268);
      goto loop_16;
    }
  }
  var_18_286 = 0;
  var_16_289 = ((u8 *) temp_17_85) + ((u32) temp_17_85->unk2C);
  if (temp_17_85->unk28 > 0)
  {
    do
    {
      var_18_286 += 1;
      func_00203B08(temp_21_115 + var_16_289->unk0, ((u8 *) temp_17_85) + ((u32) temp_17_85->unk4C), ((u8 *) var_16_289) + 0x10, ((u8 *) var_16_289) + 0x20, var_16_289->unk4);
      var_16_289 += 0x30;
    }
    while (var_18_286 < temp_17_85->unk28);
  }
  D_0015F460 = temp_21_115 + temp_17_85->unk68;
  func_00202800(((u8 *) temp_17_85) + ((u32) temp_17_85->unk5C), temp_17_85->unk58);
  func_002026C8(((u8 *) temp_17_85) + ((u32) temp_17_85->unk6C), temp_21_115 + temp_17_85->unk64, ((u8 *) temp_17_85) + ((u32) temp_17_85->unk54), temp_17_85->unk50);
  sceGsSetDefLoadImage(sp_slot, ((s32) (D_0015EE74 << 8)) >> 0x10, 4, 0, 0, 0, 0x100, 0x80);
  FlushCache(0);
  sceGsExecLoadImage(sp_slot, temp_21_115 + temp_17_85->unk84);
  func_00120558(0, 0);
  temp_7_353 = new_var << 0x13;
  D_001940C0.unk18 = (void *) (D_001940C0.unk14 + temp_22_82);
  temp_5_356 = D_0015EE74 + 0x20000;
  temp_2_359 = ((((s32) D_0015EE74) >> 8) | 0x20010000) | temp_7_353;
  D_0015EE78 = temp_5_356;
  D_0015EE74 = temp_5_356;
  D_0015EF48 = temp_2_359;
  D_001940C0.unk1C = func_001E9B10(temp_21_115 + temp_17_85->unk7C, temp_5_356, 0x20010000, temp_7_353);
  FillTransferWords(&D_0018CB20, 0, 0x1C0);
  FillTransferWords(D_00186310, 0, 0x40);
  temp_2_379 = D_001940C0.unk1C + 0x40000;
  D_0018CB20.unk58 = (s32) D_001940C0.unk1C;
  D_0018CB20.unk5C = temp_2_379;
  D_001940C0.unk1C = (s32) (temp_2_379 + 0x40000);
  var_5_385 = temp_21_115 + temp_17_85->unk80;
  var_7_388 = 0;
  if (var_5_385->unk4 != 0)
  {
    var_6_389 = ((u8 *) (&D_0018CB20)) + 0x60;
    loop_24:
    temp_2_390 = *((s32 *) var_5_385);

    var_7_388 += 1;
    var_5_385 += 8;
    *var_6_389 = (temp_21_115 + temp_17_85->unk80) + (temp_2_390 + 0x800);
    var_6_389 += 4;
    if (var_7_388 < 0x46)
    {
      if (var_5_385->unk4 != 0)
      {
        goto loop_24;
      }
    }
  }
  func_002049F0(0, var_5_385);
  var_18_411 = 0;
  D_0015EF60 = D_001940C0.unk1C;
  func_00216828(D_001940C0.unk1C, D_00137B80.unk1528, D_00137B80.unk152C);
  *((s32 *) 0x15EF64) = D_0015EF60;
  D_001940C0.unk1C = (s32) (D_0015EF60 + (D_00137B80.unk152C << 0xB));
  do
  {
    QueueDmaTransfer(var_18_411);
    var_5_435 = 0;
    if (D_001996D0.unk2C > 0)
    {
      var_3_440 = D_0015F6A0;
      do
      {
        var_5_435 += 1;
        *((s32 *) var_3_440) += D_0015F6A0 - 8;
        var_3_440 += 0x10;
      }
      while (var_5_435 < D_001996D0.unk2C);
    }
    var_18_411 += 1;
  }
  while (var_18_411 < 8);
  QueueDmaTransfer(0, var_5_435);
  if (((s32) D_001B3AC0.unk472) >= 0)
  {
    temp_4_466 = (D_001B3AC0.unk472 * 4) + D_001B3200;
    *((s32 *) (((u8 *) (*temp_4_466)) + 0x28)) = D_001861E0;
    *((u8 *) (((u8 *) (*temp_4_466)) + 0xD)) = 5;
  }
}

__attribute__((alias("FUN_001ea830"))) extern void func_001EA830(void);
#endif /* NON_MATCHING */
