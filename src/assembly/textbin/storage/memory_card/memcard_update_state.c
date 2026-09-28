#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/storage/memory_card/memcard_update_state/FUN_002093d8.s", FUN_002093d8);
#else
#include "rnc/storage_memory_card_memcard_update_state_types.h"
extern struct M2c_D_00137B80 D_00137B80;
extern u8 D_00137B94[];
extern u8 D_0013D1D0[];
extern u8 D_0013D1E8[];
extern u8 D_0013D200[];
extern u8 D_0013D220[];
extern u8 D_0013D240[];
extern u8 D_0013D270[];
extern struct M2c_D_0013D290 D_0013D290;
extern u8 D_0014EED0[];
extern u8 D_001506D0[];
extern u8 D_001516D8[];
extern u8 D_0015ED84[];
extern u8 D_0015EE90[];
extern u8 D_0015FE98[];
extern u8 D_0015FEA0[];
extern u8 D_001A04C0[];
extern u8 D_001A07C0[];
extern u8 D_001A0880[];
extern u8 D_001A08A0[];
extern u8 D_001E81E8[];
extern void DebugPrint();
extern s32 GetDmaPacketSpanBytes();
extern s32 RaiseKernelTrap();
extern s32 SceMcFormat();
extern s32 SceMcUnformat();
extern s32 func_001FD6E0();
extern s32 func_0020AE60();
extern s32 func_0020AF20();
extern s32 func_00216788();
extern s32 sceMcChdir();
extern s32 sceMcClose();
extern s32 sceMcDelete();
extern s32 sceMcGetDir();
extern s32 sceMcGetInfo();
extern s32 sceMcMkdir();
extern s32 sceMcOpen();
extern s32 sceMcRead();
extern s32 sceMcSeek();
extern s32 sceMcSync();
extern s32 sceMcWrite();
extern s32 sprintf();
extern s32 strcpy();
extern void sp40();
extern void sp48();
extern void sp50();
extern void sp58();
void memcard_update_state(void) __asm__("FUN_002093d8");

void memcard_update_state(void)
{
  u8 sp_slot[0x30];
  s32 *sp40;
  s32 sp44;
  struct M2c_sp48 *sp48;
  struct M2c_sp48 *ptr_w8;
  s32 sp4C;
  s32 *sp50;
  s32 sp54;
  struct M2c_sp58 *sp58;
  s32 sp5C;
  s32 var_7_782;
  s32 *temp_16_340;
  s32 *temp_16_357;
  s32 *temp_3_346;
  s32 *temp_3_363;
  s32 *temp_3_731;
  s32 *temp_4_1218;
  u8 *var_5_330;
  s32 *temp_4_219;
  s32 *temp_4_928;
  s32 *var_2_264;
  s32 temp_16_1652;
  s32 temp_16_1660;
  s32 temp_16_652;
  s32 temp_2_1003;
  s32 temp_2_115;
  s32 temp_2_1223;
  s32 temp_2_1645;
  s32 temp_2_186;
  s32 temp_2_215;
  s32 temp_2_262;
  s32 temp_2_328;
  s32 temp_2_44;
  s32 temp_2_463;
  s32 temp_2_574;
  s32 temp_2_733;
  s32 temp_2_803;
  s32 temp_2_81;
  s32 temp_2_867;
  s32 temp_2_977;
  int new_var;
  s32 temp_2_98;
  s32 temp_3_1070;
  s32 temp_3_1108;
  s32 temp_3_1127;
  s32 temp_3_1188;
  s32 temp_3_1204;
  s32 temp_3_1313;
  s32 temp_3_1400;
  s32 temp_3_1485;
  s32 temp_3_1605;
  s32 temp_3_1634;
  s32 temp_3_1674;
  s32 temp_3_236;
  s32 temp_3_270;
  s32 temp_3_284;
  s32 temp_3_379;
  s32 temp_3_519;
  s32 temp_3_588;
  s32 temp_3_601;
  s32 temp_3_679;
  s32 temp_3_92;
  s32 temp_4_1109;
  s32 temp_4_1189;
  s32 temp_4_126;
  s32 temp_4_1273;
  s32 temp_4_151;
  s32 temp_4_1582;
  s32 temp_4_473;
  s32 temp_4_786;
  s32 temp_4_886;
  s32 temp_5_1135;
  s32 temp_5_443;
  s32 temp_7_562;
  s32 temp_7_981;
  s32 temp_7_992;
  s32 var_2_1064;
  s32 var_2_1198;
  s32 var_2_130;
  s32 var_2_241;
  s32 var_2_467;
  s32 var_2_486;
  s32 var_2_506;
  s32 var_2_582;
  s32 var_2_638;
  s32 var_2_670;
  s32 var_2_676;
  s32 var_2_690;
  s32 var_2_695;
  s32 var_2_779;
  s32 var_2_818;
  s32 var_2_955;
  s32 var_3_590;
  s32 var_4_1020;
  s32 var_5_0;
  s32 var_5_266;
  u32 temp_4_1289;
  u32 temp_4_200;
  u32 temp_4_430;
  u32 temp_4_762;
  u32 temp_4_962;
  struct M2c_temp_16_1103 *temp_16_1103;
  struct M2c_temp_16_180 *temp_16_180;
  struct M2c_temp_16_35 *temp_16_35;
  struct M2c_temp_17_1300 *temp_17_1300;
  struct M2c_temp_17_1385 *temp_17_1385;
  struct M2c_temp_17_1470 *temp_17_1470;
  s32 *temp_19_330;
  struct M2c_temp_3_108 *temp_3_108;
  struct M2c_temp_3_127 *temp_3_127;
  struct M2c_temp_3_152 *temp_3_152;
  struct M2c_temp_3_419 *temp_3_419;
  struct M2c_temp_3_750 *temp_3_750;
  struct M2c_temp_4_1566 *temp_4_1566;
  struct M2c_temp_4_911 *temp_4_911;
  struct M2c_temp_4_91 *temp_4_91;
  struct M2c_temp_5_212 *temp_5_212;
  s32 *temp_5_926;
  struct M2c_temp_6_517 *temp_6_517;
  struct M2c_temp_6_973 *temp_6_973;
  s32 *temp_7_1059;
  s32 *temp_7_311;
  struct M2c_temp_7_60 *temp_7_60;
  struct M2c_temp_7_804 *temp_7_804;
  struct M2c_temp_7_923 *temp_7_923;
  struct M2c_temp_9_47 *temp_9_47;
  u8 *case0_p8;
  u8 *case0_pC;
  u8 *case0_p10;
  struct M2c_var_16_1055 *var_16_1055;
  struct M2c_var_16_1183 *var_16_1183;
  struct M2c_var_16_123 *var_16_123;
  register struct M2c_var_16_257 *var_16_257 asm("s0");
  struct M2c_var_16_307 *var_16_307;
  struct M2c_var_16_773 *var_16_773;
  register struct M2c_var_17_324 *var_17_324 asm("s1");
  struct M2c_var_2_511 *var_2_511;
  register struct M2c_var_2_744 * var_2_744 asm("s0");
  struct M2c_var_3_1373 *var_3_1373;
  struct M2c_var_3_504 *var_3_504;
  s32 *var_3_625;
  struct M2c_var_4_141 *var_4_141;
  struct M2c_var_4_441 *var_4_441;
  register struct M2c_var_5_269 * var_5_269 asm("s0");
  struct M2c_var_5_472 *var_5_472;
  u8 frame_pad[0x10];
  if (D_0013D290.unkCC != 0)
  {
    D_0013D290.unkCC = (s32) (sceMcSync(1, ((u8 *) (&D_0013D290)) + 0xB8, ((u8 *) (&D_0013D290)) + 0xBC) == 0);
  }
  else
  {
    D_0013D290.unkCC = 1;
    switch ((u32) D_0013D290.unkD4)
    {
      case 0:
        temp_16_35 = &D_0013D290;
        if (((struct M2c_temp_16_35 *)&D_0013D290)->unkC4 < 0)
      {
        ((struct M2c_temp_16_35 *)&D_0013D290)->unkC4 = 0;
      }
        case0_p8 = ((u8 *) ((struct M2c_temp_16_35 *)&D_0013D290)) + 8;
        case0_pC = ((u8 *) ((struct M2c_temp_16_35 *)&D_0013D290)) + 0xC;
        case0_p10 = ((u8 *) ((struct M2c_temp_16_35 *)&D_0013D290)) + 0x10;
        temp_2_44 = ((struct M2c_temp_16_35 *)&D_0013D290)->unkC4 * 0xB8;
        if (sceMcGetInfo(*((s32 *) (((u8 *) ((struct M2c_temp_16_35 *)&D_0013D290)) + temp_2_44)), *((s32 *) (((u8 *) ((struct M2c_temp_16_35 *)&D_0013D290)) + temp_2_44 + 4)), case0_p8 + temp_2_44, case0_pC + temp_2_44, case0_p10 + temp_2_44) == 0)
      {
        ((struct M2c_temp_16_35 *)&D_0013D290)->unkD4 = 1;
        return;
      }
        return;

      case 1:
        temp_7_60 = &D_0013D290;
        if (temp_7_60->unkBC != 0)
      {
        *((s32 *) (((u8 *) (((u8 *) temp_7_60) + (temp_7_60->unkC4 * 0xB8))) + 0x14)) = -3;
        *((s32 *) (((u8 *) (((u8 *) temp_7_60) + (temp_7_60->unkC4 * 0xB8))) + 0x1C)) = (s32) temp_7_60->unkBC;
        *((s32 *) (((u8 *) (((u8 *) temp_7_60) + (temp_7_60->unkC4 * 0xB8))) + 0xAC)) = -1;
      }
        temp_2_81 = temp_7_60->unkC4 + 1;
        temp_7_60->unkC4 = temp_2_81;
        if (temp_2_81 <= 0)
      {
        temp_7_60->unkD4 = 0;
        goto block_351;
      }
        temp_7_60->unkD4 = 2;
        goto block_351;

      default:
        break;

      case 2:
        temp_4_91 = &D_0013D290;
        temp_3_92 = temp_4_91->unkDC;
        if (temp_3_92 >= 0)
      {
        if (temp_4_91->unk1C == 0)
        {
          temp_2_98 = temp_4_91->unkE0;
          if (temp_2_98 >= 0)
          {
            temp_4_91->unkC4 = temp_2_98;
            temp_4_91->unkD4 = temp_3_92;
            temp_4_91->unkE4 = 0;
          }
          else
          {
            temp_4_91->unkE4 = 0x271A;
          }
        }
        temp_3_108 = &D_0013D290;
        temp_3_108->unkDC = -1;
        temp_3_108->unkE0 = -1;
      }
      else
      {
        temp_2_115 = temp_4_91->unkC4 + 1;
        temp_4_91->unkC4 = temp_2_115;
        if (temp_2_115 < 0x1F)
        {
          goto block_351;
        }
        temp_4_91->unkD4 = 0;
        temp_4_91->unkC4 = 0;
      }
        goto block_351;

      case 3:
        var_16_123 = &D_0013D290;
        temp_4_126 = var_16_123->unkC4 * 0xB8;
        temp_3_127 = ((u8 *) var_16_123) + temp_4_126;
        var_2_130 = 2;
        if (temp_3_127->unk10 == 0)
      {
        if (SceMcFormat(*((s32 *) (((u8 *) var_16_123) + temp_4_126)), temp_3_127->unk4) == 0)
        {
          var_16_123->unkD4 = 4;
          return;
        }
      }
      else
      {
        block_278:
        var_16_123->unkCC = 0;

        var_16_123->unkD4 = var_2_130;
      }
        break;

      case 4:
        var_4_141 = &D_0013D290;
        if (var_4_141->unkBC != 0)
      {
        block_34:
        var_4_141->unkE4 = 1;

        var_4_141->unkE8 = (s32) var_4_141->unkC4;
      }
        block_35:
      var_4_141->unkCC = 0;

        var_4_141->unkD4 = 0;
        var_4_141->unkC4 = 0;
        break;

      case 5:
        var_16_123 = &D_0013D290;
        temp_4_151 = var_16_123->unkC4 * 0xB8;
        temp_3_152 = ((u8 *) var_16_123) + temp_4_151;
        var_2_130 = 2;
        if (temp_3_152->unk10 == 0)
      {
        if (SceMcUnformat(*((s32 *) (((u8 *) var_16_123) + temp_4_151)), temp_3_152->unk4) == 0)
        {
          var_16_123->unkD4 = 6;
          return;
        }
      }
      else
      {
        goto block_278;
      }
        break;

      case 6:
        var_4_141 = &D_0013D290;
        if (var_4_141->unkBC != 0)
      {
        var_4_141->unkE4 = 2;
        var_4_141->unkE8 = (s32) var_4_141->unkC4;
      }
        var_4_141->unkD4 = 0;
        var_4_141->unkC4 = 0;
        var_4_141->unkCC = 0;
        break;

      case 7:
        temp_16_180 = &D_0013D290;
        temp_2_186 = ((struct M2c_temp_16_180 *)&D_0013D290)->unkC4 * 0xB8;
        if (sceMcChdir(*((s32 *) (((u8 *) ((struct M2c_temp_16_180 *)&D_0013D290)) + temp_2_186)), *((s32 *) (((u8 *) (((u8 *) ((struct M2c_temp_16_180 *)&D_0013D290)) + temp_2_186)) + 0x4)), D_0013D1D0, 0) == 0)
      {
        ((struct M2c_temp_16_180 *)&D_0013D290)->unkD8 = 0;
        ((struct M2c_temp_16_180 *)&D_0013D290)->unkD4 = 8;
        return;
      }
        break;

      case 8:
        temp_4_200 = D_0013D290.unkD8;
        switch (temp_4_200)
      {
        case 0:
          temp_5_212 = &D_0013D290;
          temp_2_215 = ((struct M2c_temp_5_212 *)&D_0013D290)->unkC4;
          if (((struct M2c_temp_5_212 *)&D_0013D290)->unkBC == 0)
        {
          temp_4_219 = (temp_2_215 * 0xB8) + (((u8 *) ((struct M2c_temp_5_212 *)&D_0013D290)) + 0x14);
          if ((*temp_4_219) < 0)
          {
            *temp_4_219 = -1;
          }
          ((struct M2c_temp_5_212 *)&D_0013D290)->unkCC = 0;
          ((struct M2c_temp_5_212 *)&D_0013D290)->unkD8 = 1;
        }
        else
        {
          *((s32 *) (((u8 *) (((u8 *) ((struct M2c_temp_5_212 *)&D_0013D290)) + (temp_2_215 * 0xB8))) + 0x14)) = -2;
          temp_3_236 = ((struct M2c_temp_5_212 *)&D_0013D290)->unkBC;
          if (temp_3_236 == (-2))
          {
            var_2_241 = 3;
            goto block_48;
          }
          var_2_241 = 4;
          if (temp_3_236 != (-4))
          {
            block_48:
            ((struct M2c_temp_5_212 *)&D_0013D290)->unkE4 = var_2_241;

            ((struct M2c_temp_5_212 *)&D_0013D290)->unkE8 = ((struct M2c_temp_5_212 *)&D_0013D290)->unkC4;
          }
          block_388:
          var_2_744 = &D_0013D290;

          var_2_744->unkCC = 0;
          block_389:
          var_2_744->unkD4 = 0;

          var_2_744->unkC4 = 0;
        }
          break;

        case 1:
          sprintf(sp_slot, D_0013D270, 0);
          var_16_257 = &D_0013D290;
          temp_2_262 = var_16_257->unkC4 * 0xB8;
          var_2_264 = ((u8 *) var_16_257) + temp_2_262;
          var_5_266 = *((s32 *) (((u8 *) (((u8 *) var_16_257) + temp_2_262)) + 0x4));
          block_207:
        var_2_670 = sceMcOpen(*var_2_264, var_5_266, sp_slot, 1);

          block_208:
        if (var_2_670 == 0)
        {
          var_2_582 = var_16_257->unkD8;
          block_210:
          var_16_257->unkD8 = (s32) (var_2_582 + 1);

        }

          break;

        case 2:
          var_5_269 = &D_0013D290;
          temp_3_270 = var_5_269->unkBC;
          if (temp_3_270 < 0)
        {
          temp_3_284 = var_5_269->unkBC;
          *((s32 *) (((u8 *) (((u8 *) var_5_269) + (var_5_269->unkC4 * 0xB8))) + 0x14)) = -2;
          var_5_269->unkE8 = (s32) var_5_269->unkC4;
          if (temp_3_284 == (-7))
          {
            var_5_269->unkE4 = 0x2710;
            goto block_297;
          }
          if (temp_3_284 == (-5))
          {
            var_5_269->unkE4 = 0x2711;
            goto block_297;
          }
          if (temp_3_284 == (-4))
          {
            var_5_269->unkE4 = 0x2712;
            goto block_297;
          }
          if (temp_3_284 == (-3))
          {
            var_5_269->unkE4 = 0x2713;
            goto block_297;
          }
          if (temp_3_284 != (-2))
          {
            var_5_269->unkE4 = 4;
            goto block_297;
          }
          var_5_269->unkE4 = 3;
          goto block_297;

          block_297:
          block_298:
          var_2_511 = &D_0013D290;

          var_2_511->unkC4 = 0;
          var_2_511->unkD4 = 0;
        }
        else
        {
          var_5_269->unkD8 = 3;
          var_5_269->unkD0 = temp_3_270;
          block_215:
          goto block_351;
        }
          goto block_352;

        case 3:
          var_16_307 = &D_0013D290;
          var_16_307->unkF0 = 8;
          temp_7_311 = ((u8 *) var_16_307) + 0xB0;
          if (sceMcRead(var_16_307->unkD0, (var_16_307->unkC4 * 0xB8) + ((u8 *) temp_7_311), 8) == 0)
        {
          block_185:
          var_16_307->unkD8 = 4;

        }
          break;

        case 4:
          var_17_324 = &D_0013D290;
          temp_2_328 = var_17_324->unkC4;
          if (var_17_324->unkBC == var_17_324->unkF0)
        {
          var_5_330 = ((u8 *) var_17_324) + 0xAC;
          *((s32 *) (((u8 *) var_5_330) + (temp_2_328 * 0xB8))) = 0;
          if ((*((s32 *) ((((u8 *) var_17_324) + (var_17_324->unkC4 * 0xB8)) + 0xB0))) != GetDmaPacketSpanBytes(D_001A04C0))
          {
            *((s32 *) (((u8 *) var_5_330) + (var_17_324->unkC4 * 0xB8))) += 1;
          }
          if ((*((s32 *) ((((u8 *) var_17_324) + (var_17_324->unkC4 * 0xB8)) + 0xB4))) != GetDmaPacketSpanBytes(D_001A07C0))
          {
            *((s32 *) (((u8 *) var_5_330) + (var_17_324->unkC4 * 0xB8))) += 1;
          }
          var_17_324->unkCC = 0;
          var_17_324->unkD8 = 5;
        }
        else
        {
          var_5_0 = temp_2_328 * 0xB8;
          *((s32 *) (((u8 *) (((u8 *) var_17_324) + var_5_0)) + 0x14)) = -2;
          temp_3_379 = var_17_324->unkBC;
          var_17_324->unkE8 = (s32) var_17_324->unkC4;
          if (temp_3_379 >= 0)
          {
            var_17_324->unkE4 = 0x2716;
            header_transfer_close:
            if (sceMcClose(var_17_324->unkD0) == 0)
            {
              block_393:
              var_17_324->unkCC = 0;

              var_17_324->unkD4 = 0;
              var_17_324->unkC4 = 0;
            }
          }
          else
          {
            if (temp_3_379 == (-5))
            {
              var_17_324->unkE4 = 0x2711;
              goto block_387;
            }
            if (temp_3_379 == (-4))
            {
              var_17_324->unkE4 = 0x2714;
              goto block_387;
            }
            if (temp_3_379 == (-3))
            {
              var_17_324->unkE4 = 0x2715;
              goto block_387;
            }
            if (temp_3_379 == (-2))
            {
              var_17_324->unkE4 = 3;
              goto block_387;
            }
            var_17_324->unkE4 = 4;
            block_387:
            var_17_324->unkCC = 0;
            var_17_324->unkD4 = 0;
            var_17_324->unkC4 = 0;
          }
        }
          break;

        case 5:
          var_16_123 = &D_0013D290;
          if (sceMcClose(var_16_123->unkD0) == 0)
        {
          var_2_130 = 0x15;
          goto block_278;
        }
          break;

      }

        break;

      case 9:
        temp_3_419 = &D_0013D290;
        var_5_0 = 0xB8;
        temp_3_419->unkD4 = 0xA;
        temp_3_419->unkD8 = 0;
        *((s32 *) (((u8 *) (((u8 *) temp_3_419) + (temp_3_419->unkC4 * 0xB8))) + 0x14)) = 0;

      case 10:
        temp_4_430 = D_0013D290.unkD8;
        switch (temp_4_430)
      {
        case 0:
          var_4_441 = &D_0013D290;
          temp_5_443 = var_4_441->unkC4;
          if ((*((s32 *) (((u8 *) (((u8 *) var_4_441) + (temp_5_443 * 0xB8))) + 0xC))) >= 0x15E)
        {
          var_4_441->unkD8 = 1;
        }
        else
        {
          var_4_441->unkE8 = temp_5_443;
          var_4_441->unkE4 = 7;
          block_154:
          var_4_441->unkC4 = 0;

          var_4_441->unkD4 = 0;
        }
          goto block_351;

        case 1:
          var_16_257 = &D_0013D290;
          temp_2_463 = var_16_257->unkC4 * 0xB8;
          var_2_467 = sceMcMkdir(*((s32 *) (((u8 *) var_16_257) + temp_2_463)), *((s32 *) (((u8 *) (((u8 *) var_16_257) + temp_2_463)) + 0x4)), D_0013D1D0);
          block_167:
        if (var_2_467 == 0)
        {
          var_16_257->unkD8 = 2;
          return;
        }

          break;

        case 2:
          var_5_472 = &D_0013D290;
          temp_4_473 = var_5_472->unkBC;
          switch (temp_4_473)
        {
          case -4:

          case 0:
            var_5_472->unkD8 = 3;
            break;

          default:
            var_5_472->unkE8 = (s32) var_5_472->unkC4;
          if (temp_4_473 == (-3))
          {
            var_5_472->unkE4 = 7;
            goto block_194;
          }
          if (temp_4_473 == (-2))
          {
            var_5_472->unkE4 = 6;
            goto block_194;
          }
          var_5_472->unkE4 = 0xD;
          goto block_194;
            block_194:

            goto block_388;

        }

          break;

        case 3:
          func_001FD6E0(D_00137B80.unk14 << 0xB, &sp40, &sp44);
          func_00216788(sp40, D_00137B80.unk10, D_00137B80.unk14);
          var_3_504 = &D_0013D290;
          var_2_506 = 4;
          block_348:
        var_3_504->unkD8 = var_2_506;

          var_3_504->unkCC = 0;
          break;

        case 4:
          var_2_511 = &D_0013D290;
          if (*((s16 *) ((u8 *)0x1516D8)) == 0)
        {
          D_0013D290.unkD8 = 5;
          goto block_351;
        }
          goto block_352;

        case 5:

        case 9:

        case 13:

        case 18:
          temp_6_517 = &D_0013D290;
          temp_3_519 = temp_6_517->unkD8;
          switch (temp_3_519)
        {
          case 9:
            strcpy(sp_slot, D_0013D220);
            goto block_112;

          case 5:
            strcpy(sp_slot, D_0013D200);
            goto block_112;

          case 0xD:
            temp_7_562 = temp_6_517->unkC4 * 0xB8;
            sprintf(sp_slot, D_0013D270, *((s32 *) (((u8 *) (((u8 *) temp_6_517) + temp_7_562)) + 0x14)));
            goto block_112;

          case 0x12:
            strcpy(sp_slot, D_0013D240);
            goto block_112;

        }
          block_112:
          var_16_257 = &D_0013D290;
          temp_2_574 = var_16_257->unkC4 * 0xB8;
          if (sceMcOpen(*((s32 *) (((u8 *) var_16_257) + temp_2_574)), *((s32 *) (((u8 *) (((u8 *) var_16_257) + temp_2_574)) + 0x4)), sp_slot, 0x203) == 0)
        {
          var_2_582 = var_16_257->unkD8;
          var_16_257->unkD0 = -1;
          goto block_210;
        }
          break;

        case 6:

        case 10:

        case 14:

        case 19:
          var_17_324 = &D_0013D290;
          temp_3_588 = var_17_324->unkBC;
          if (temp_3_588 >= 0)
        {
          if (var_17_324->unkD0 < 0)
        {
          var_17_324->unkD0 = temp_3_588;
        }
          func_001FD6E0((*((s32 *) D_00137B94)) << 0xB, &sp48, &sp4C);
          temp_3_601 = var_17_324->unkD8;
          if (temp_3_601 != 0xA)
        {
          if (temp_3_601 < 0xB)
          {
            var_16_257 = &D_0013D290;
            if (temp_3_601 != 6)
            {
            }
            else
            {
              ptr_w8 = sp48;
              var_17_324->unkF0 = 0x3C4;
              var_3_625 = ((u32) ptr_w8) + ptr_w8->unk0;
              goto block_133;
            }
          }
          else
            if (temp_3_601 != 0xE)
          {
            var_16_257 = &D_0013D290;
            if (temp_3_601 != 0x13)
            {
            }
            else
            {
              if (*((s32 *) ((u8 *)0x15EE90)) == 0)
              {
                var_17_324->unkF0 = 0x3C04;
              }
              else
              {
                var_17_324->unkF0 = 0x3C00;
              }
              *((s32 *) (((u8 *) (&D_0013D290)) + 0xEC)) = D_0015ED84;
              goto block_134;
            }
          }
          else
          {
            temp_16_652 = GetDmaPacketSpanBytes(D_001A04C0);
            var_17_324->unkF0 = (s32) ((temp_16_652 + (GetDmaPacketSpanBytes(D_001A07C0) * 0x14)) + 8);
            ptr_w8 = sp48;
            var_3_625 = ((u32) ptr_w8) + ptr_w8->unk10;
            block_133:
            var_17_324->unkEC = var_3_625;

            goto block_134;
          }
        }
        else
        {
          ptr_w8 = sp48;
          var_17_324->unkF0 = (s32) ptr_w8->unkC;
          var_17_324->unkEC = (void *) (((u32) ptr_w8) + ptr_w8->unk8);
          block_134:
          var_16_257 = &D_0013D290;

        }
          var_2_670 = sceMcWrite(var_16_257->unkD0, var_16_257->unkEC, var_16_257->unkF0);
          goto block_208;
        }
        else
        {
          var_3_590 = var_17_324->unkC4;
          var_2_676 = 0xA;
          block_392:
          var_17_324->unkE4 = var_2_676;

          var_17_324->unkE8 = var_3_590;
          goto block_393;
        }

        case 7:

        case 11:

        case 15:

        case 20:
          var_16_257 = &D_0013D290;
          temp_3_679 = var_16_257->unkBC;
          if (temp_3_679 == var_16_257->unkF0)
        {
          var_2_670 = sceMcClose(var_16_257->unkD0);
          goto block_208;
        }
        else
        {
          var_16_257->unkE8 = (s32) var_16_257->unkC4;
          var_2_690 = 0xB;
          if (temp_3_679 >= 0)
          {
            var_16_257->unkE4 = var_2_690;
            if (sceMcClose(var_16_257->unkD0) == 0)
            {
              var_16_257->unkCC = 0;
              var_16_257->unkD4 = 0;
              var_16_257->unkC4 = 0;
            }
          }
          else
          {
          switch (temp_3_679)
          {
            case -4:
              var_16_257->unkE4 = 8;
              block_148:

              var_2_744 = &D_0013D290;
              var_2_744->unkCC = 0;
              var_2_744->unkD4 = 0;
              var_2_744->unkC4 = 0;

            case -3:
              var_16_257->unkE4 = 7;
              goto block_148;

            default:
              if (temp_3_679 == (-2))
              {
                var_16_257->unkE4 = 6;
                goto block_148;
              }
              var_16_257->unkE4 = 0xD;
              goto block_148;
              goto block_148;

          }
          }

        }
          break;

        case 8:

        case 12:

        case 16:
          var_4_441 = &D_0013D290;
          if (var_4_441->unkBC != 0)
        {
          var_4_441->unkE4 = 0xC;
          var_4_441->unkE8 = (s32) var_4_441->unkC4;
          goto block_154;
        }
          var_4_441->unkD8 = (s32) (var_4_441->unkD8 + 1);
          goto block_351;

        case 17:
          var_5_269 = &D_0013D290;
          temp_3_731 = (var_5_269->unkC4 * 0xB8) + (((u8 *) var_5_269) + 0x14);
          temp_2_733 = (*temp_3_731) + 1;
          *temp_3_731 = temp_2_733;
          if (temp_2_733 >= 5)
        {
          var_5_269->unkD8 = var_5_269->unkD8 + 1;
        }
        else
        {
          var_5_269->unkD8 = 0xD;
        }
          goto block_215;

        case 21:
          var_2_744 = &D_0013D290;
          var_2_744->unk14 = -1;
          var_2_744->unkAC = 0;
          goto block_389;

      }

        break;

      case 11:
        temp_3_750 = &D_0013D290;
        temp_3_750->unkD4 = 0xC;
        temp_3_750->unkD8 = 0;
        *((s32 *) (((u8 *) (((u8 *) temp_3_750) + (temp_3_750->unkC4 * 0xB8))) + 0x14)) = -2;

      case 12:
        temp_4_762 = D_0013D290.unkD8;
        switch (temp_4_762)
      {
        case 0:
          var_16_773 = &D_0013D290;
          var_2_779 = var_16_773->unkC4 * 0xB8;
          var_7_782 = 0;
          block_181:
        if (sceMcGetDir(*((s32 *) (((u8 *) var_16_773) + var_2_779)), *((s32 *) (((u8 *) (((u8 *) var_16_773) + var_2_779)) + 0x4)), D_0013D1E8, var_7_782, 1, D_001A0880) == 0)
        {
          var_16_773->unkD8 = 1;
          return;
        }

          break;

        case 1:
          var_16_257 = &D_0013D290;
          temp_4_786 = var_16_257->unkBC;
          switch (temp_4_786)
        {
          case 1:
            sprintf(sp_slot, D_0015FE98, D_0013D1D0, (s32) D_001A08A0);
            DebugPrint(D_0015FEA0, sp_slot);
            temp_2_803 = var_16_257->unkC4 * 0xB8;
            temp_7_804 = ((u8 *) var_16_257) + temp_2_803;
            var_2_467 = sceMcDelete(*((s32 *) (((u8 *) var_16_257) + temp_2_803)), temp_7_804->unk4, sp_slot, temp_7_804);
            goto block_167;

          case 0:
            var_16_257->unkD8 = 3;
            block_173:

            var_2_511 = &D_0013D290;
            var_2_511->unkCC = 0;
            break;

          default:
            var_16_257->unkE8 = (s32) var_16_257->unkC4;
            if (temp_4_786 == (-5))
            {
              var_16_257->unkE4 = 0x10;
              goto block_298;
            }
            if (temp_4_786 == (-4))
            {
              var_16_257->unkE4 = 0xF;
              goto block_298;
            }
            if (temp_4_786 == (-2))
            {
              var_16_257->unkE4 = 0xE;
              goto block_298;
            }
            var_16_257->unkE4 = 0x12;
            goto block_298;

        }

          break;

        case 2:
          var_16_773 = &D_0013D290;
          var_2_779 = var_16_773->unkC4 * 0xB8;
          var_7_782 = 1;
        if (sceMcGetDir(*((s32 *) (((u8 *) var_16_773) + var_2_779)), *((s32 *) (((u8 *) (((u8 *) var_16_773) + var_2_779)) + 0x4)), D_0013D1E8, var_7_782, 1, D_001A0880) == 0)
        {
          var_16_773->unkD8 = 1;
          return;
        }
          break;

        case 3:
          var_16_307 = &D_0013D290;
          temp_2_867 = var_16_307->unkC4 * 0xB8;
          if (sceMcDelete(*((s32 *) (((u8 *) var_16_307) + temp_2_867)), *((s32 *) (((u8 *) (((u8 *) var_16_307) + temp_2_867)) + 0x4)), D_0013D1D0) == 0)
        {
          DebugPrint(D_001E81E8, D_0013D1D0);
          goto block_185;
        }
          break;

        case 4:
          var_5_472 = &D_0013D290;
          temp_4_886 = var_5_472->unkBC;
          if (temp_4_886 != 0)
        {
          var_5_472->unkE8 = (s32) var_5_472->unkC4;
          if (temp_4_886 == (-6))
          {
            var_5_472->unkE4 = 0x11;
            goto block_194;
          }
          if (temp_4_886 == (-5))
          {
            var_5_472->unkE4 = 0x10;
            goto block_194;
          }
          if (temp_4_886 == (-4))
          {
            var_5_472->unkE4 = 0xF;
            goto block_194;
          }
          if (temp_4_886 == (-2))
          {
            var_5_472->unkE4 = 0xE;
            goto block_194;
          }
          var_5_472->unkE4 = 0x12;
          goto block_194;
        }
          goto block_388;

      }

        break;

      case 21:
        temp_4_911 = &D_0013D290;
        temp_4_911->unkD4 = 0x16;
        *((s32 *) (((u8 *) (((u8 *) temp_4_911) + (temp_4_911->unkC4 * 0xB8))) + 0x18)) = -1;
        temp_4_911->unkC8 = 0;
        break;

      case 22:
        temp_7_923 = &D_0013D290;
        temp_5_926 = ((u8 *) temp_7_923) + 0x18;
        temp_4_928 = (temp_7_923->unkC4 * 0xB8) + ((u8 *) temp_5_926);
        *temp_4_928 += 1;
        if ((*((s32 *) ((temp_7_923->unkC4 * 0xB8) + ((u8 *) temp_5_926)))) < 5)
      {
        temp_7_923->unkD8 = 0;
        temp_7_923->unkD4 = 0x17;
      }
      else
      {
        temp_7_923->unkD4 = 0;
        temp_7_923->unkC4 = 0;
        temp_7_923->unkCC = 0;
      }
        break;

      case 13:
        var_5_0 = (s32) (&D_0013D290);
        var_2_955 = 0x13;
        if ((*((s32 *) (((u8 *) (((*((s32 *) (((u8 *) var_5_0) + 0xC4))) * 0xB8) + var_5_0)) + 0x14))) >= 0)
      {
        *((s32 *) (((u8 *) var_5_0) + 0xC8)) = 0;
        *((s32 *) (((u8 *) var_5_0) + 0xD8)) = 0;
        *((s32 *) (((u8 *) var_5_0) + 0xD4)) = 0xE;
        case 14:
          case 23:
          temp_4_962 = D_0013D290.unkD8;


        switch (temp_4_962)
        {
          case 0:
            temp_6_973 = &D_0013D290;
            temp_2_977 = temp_6_973->unkC4;
            if (temp_6_973->unkD4 == 0x17)
          {
            temp_7_981 = temp_2_977 * 0xB8;
            sprintf(sp_slot, D_0013D270, *((s32 *) (((u8 *) (((u8 *) temp_6_973) + temp_7_981)) + 0x18)), temp_7_981);
          }
          else
          {
            temp_7_992 = temp_2_977 * 0xB8;
            sprintf(sp_slot, D_0013D270, *((s32 *) (((u8 *) (((u8 *) temp_6_973) + temp_7_992)) + 0x14)), temp_7_992);
          }
            var_16_257 = &D_0013D290;
            temp_2_1003 = var_16_257->unkC4 * 0xB8;
            var_2_264 = ((u8 *) var_16_257) + temp_2_1003;
            var_5_266 = *((s32 *) (((u8 *) (((u8 *) var_16_257) + temp_2_1003)) + 0x4));
            goto block_207;

          case 1:
            var_5_269 = &D_0013D290;
            var_4_1020 = var_5_269->unkBC;
            if (var_4_1020 < 0)
          {
            var_5_269->unkE8 = (s32) var_5_269->unkC4;
            if (var_4_1020 == (-7))
            {
              var_5_269->unkE4 = 0x14;
              goto block_297;
            }
            if (var_4_1020 == (-5))
            {
              var_5_269->unkE4 = 0x15;
              goto block_297;
            }
            if (var_4_1020 == (-4))
            {
              var_5_269->unkE4 = 0x16;
              goto block_297;
            }
            if (var_4_1020 == (-3))
            {
              var_5_269->unkE4 = 0x17;
              goto block_297;
            }
            if (var_4_1020 == (-2))
            {
              var_5_269->unkE4 = 0x18;
              goto block_297;
            }
            var_5_269->unkE4 = 0x1C;
            goto block_297;
          }
            block_214:
          var_5_269->unkD8 = 2;

            var_5_269->unkD0 = var_4_1020;
            goto block_215;
            goto block_352;

          case 2:
            var_16_1055 = &D_0013D290;
            var_16_1055->unkF0 = 8;
            temp_7_1059 = ((u8 *) var_16_1055) + 0xB0;
            var_2_1064 = sceMcRead(var_16_1055->unkD0, (var_16_1055->unkC4 * 0xB8) + ((u8 *) temp_7_1059), 8);
            block_300:
          if (var_2_1064 == 0)
          {
            var_16_1055->unkD8 = 3;
            return;
          }

            break;

          case 3:
            var_16_257 = &D_0013D290;
            temp_3_1070 = var_16_257->unkBC;
            if (temp_3_1070 == var_16_257->unkF0)
          {
            var_16_257->unkCC = 0;
            var_16_257->unkD8 = 4;
          }
          else
          {
            var_16_257->unkE8 = (s32) var_16_257->unkC4;
            if (temp_3_1070 >= 0)
            {
              var_2_690 = 0x1B;
              var_16_257->unkE4 = var_2_690;
              if (sceMcClose(var_16_257->unkD0) == 0)
              {
                var_16_257->unkCC = 0;
                var_16_257->unkD4 = 0;
                var_16_257->unkC4 = 0;
              }
            }
            else
            {
              if (temp_3_1070 == (-5))
              {
                var_16_257->unkE4 = 0x15;
                goto block_148;
              }
              if (temp_3_1070 == (-4))
              {
                var_16_257->unkE4 = 0x19;
                goto block_148;
              }
              if (temp_3_1070 == (-3))
              {
                var_16_257->unkE4 = 0x1A;
                goto block_148;
              }
              if (temp_3_1070 == (-2))
              {
                var_16_257->unkE4 = 0x18;
                goto block_148;
              }
              var_16_257->unkE4 = 0x1C;
              goto block_148;
            }
          }
            break;

          case 4:
            temp_16_1103 = &D_0013D290;
            temp_3_1108 = *((s32 *) (((u8 *) (((u8 *) temp_16_1103) + (temp_16_1103->unkC4 * 0xB8))) + 0xB0));
            temp_4_1109 = temp_3_1108 < 0x1801;
            temp_16_1103->unkF0 = temp_3_1108;
            if (temp_4_1109 == 0)
          {
            RaiseKernelTrap(temp_4_1109);
          }
            if (sceMcRead(temp_16_1103->unkD0, D_0014EED0, temp_16_1103->unkF0) == 0)
          {
            temp_16_1103->unkD8 = 5;
            return;
          }
            break;

          case 5:
            var_16_257 = &D_0013D290;
            temp_3_1127 = var_16_257->unkBC;
            if (temp_3_1127 == var_16_257->unkF0)
          {
            if (var_16_257->unkD4 == 0x17)
            {
              temp_5_1135 = var_16_257->unkC4;
              func_0020AE60(D_0014EED0, temp_5_1135, *((s32 *) (((u8 *) (((u8 *) var_16_257) + (temp_5_1135 * 0xB8))) + 0x18)));
              var_16_257->unkD8 = 8;
              goto block_173;
            }
            *((s32 *) (((u8 *) (((u8 *) var_16_257) + (var_16_257->unkC4 * 0xB8))) + 0xAC)) = func_0020AF20(D_0014EED0, 0, D_001A04C0);
            var_16_257->unkD8 = 6;
            goto block_351;
          }
          else
          {
            var_16_257->unkE8 = (s32) var_16_257->unkC4;
            var_2_690 = 0x1B;
            if (temp_3_1127 >= 0)
            {
              var_2_690 = 0x1B;
              var_16_257->unkE4 = var_2_690;
              if (sceMcClose(var_16_257->unkD0) == 0)
              {
                var_16_257->unkCC = 0;
                var_16_257->unkD4 = 0;
                var_16_257->unkC4 = 0;
              }
            }
            else
            {
              if (temp_3_1127 == (-5))
              {
                var_16_257->unkE4 = 0x15;
                goto block_148;
              }
              if (temp_3_1127 == (-4))
              {
                var_16_257->unkE4 = 0x19;
                goto block_148;
              }
              if (temp_3_1127 == (-3))
              {
                var_16_257->unkE4 = 0x1A;
                goto block_148;
              }
              if (temp_3_1127 == (-2))
              {
                var_16_257->unkE4 = 0x18;
                goto block_148;
              }
              var_16_257->unkE4 = 0x1C;
              goto block_148;
            }

          }
            break;

          case 6:
            var_16_1183 = &D_0013D290;
            temp_3_1188 = *((s32 *) (((u8 *) (((u8 *) var_16_1183) + (var_16_1183->unkC4 * 0xB8))) + 0xB4));
            temp_4_1189 = temp_3_1188 < 0x1001;
            var_16_1183->unkF0 = temp_3_1188;
            if (temp_4_1189 == 0)
          {
            RaiseKernelTrap(temp_4_1189);
          }
            var_2_1198 = sceMcRead(var_16_1183->unkD0, D_001506D0, var_16_1183->unkF0);
            block_322:
          if (var_2_1198 == 0)
          {
            var_16_1183->unkD8 = 7;
            return;
          }

            break;

          case 7:
            var_16_257 = &D_0013D290;
            temp_3_1204 = var_16_257->unkBC;
            if (temp_3_1204 == var_16_257->unkF0)
          {
            temp_4_1218 = (var_16_257->unkC4 * 0xB8) + (((u8 *) var_16_257) + 0xAC);
            *temp_4_1218 += func_0020AF20(D_001506D0, var_16_257->unkC8, D_001A07C0);
            temp_2_1223 = var_16_257->unkC8 + 1;
            var_16_257->unkC8 = temp_2_1223;
            if (temp_2_1223 < 0x14)
            {
              var_16_257->unkD8 = 6;
              goto block_173;
            }
            var_16_257->unkD8 = 8;
            goto block_173;
            goto block_173;
          }
          else
          {
            var_16_257->unkE8 = (s32) var_16_257->unkC4;
            var_2_690 = 0x1B;
            if (temp_3_1204 >= 0)
            {
              var_2_690 = 0x1B;
              var_16_257->unkE4 = var_2_690;
              if (sceMcClose(var_16_257->unkD0) == 0)
              {
                var_16_257->unkCC = 0;
                var_16_257->unkD4 = 0;
                var_16_257->unkC4 = 0;
              }
            }
            else
            {
              if (temp_3_1204 == (-5))
              {
                var_16_257->unkE4 = 0x15;
                goto block_148;
              }
              if (temp_3_1204 == (-4))
              {
                var_16_257->unkE4 = 0x19;
                goto block_148;
              }
              if (temp_3_1204 == (-3))
              {
                var_16_257->unkE4 = 0x1A;
                goto block_148;
              }
              if (temp_3_1204 == (-2))
              {
                var_16_257->unkE4 = 0x18;
                goto block_148;
              }
              var_16_257->unkE4 = 0x1C;
              goto block_148;
            }

          }
            break;

          case 8:
            var_16_123 = &D_0013D290;
            if (sceMcClose(var_16_123->unkD0) == 0)
          {
            if (var_16_123->unkD4 == 0x17)
            {
              var_2_130 = 0x16;
              goto block_278;
            }
            block_346:
            var_16_123->unkD4 = 0;

            var_16_123->unkC4 = 0;
          }
            break;

        }

      }
      else
      {
        block_356:
        *((s32 *) (((u8 *) var_5_0) + 0xCC)) = 0;

        *((s32 *) (((u8 *) var_5_0) + 0xE4)) = var_2_955;
        *((s32 *) (((u8 *) var_5_0) + 0xD4)) = 0;
        *((s32 *) (((u8 *) var_5_0) + 0xC4)) = 0;
      }
        break;

      case 15:
        var_5_0 = (s32) (&D_0013D290);
        temp_4_1273 = (*((s32 *) (((u8 *) var_5_0) + 0xC4))) * 0xB8;
        if ((*((s32 *) (((u8 *) (var_5_0 + temp_4_1273)) + 0xAC))) != 0)
      {
        var_2_955 = 0x2717;
        goto block_356;
      }
        var_2_955 = 0x1D;
        if ((*((s32 *) (((u8 *) (var_5_0 + temp_4_1273)) + 0x14))) >= 0)
      {
        *((s32 *) (((u8 *) var_5_0) + 0xD8)) = 0;
        *((s32 *) (((u8 *) var_5_0) + 0xD4)) = 0x10;
        case 16:
          temp_4_1289 = D_0013D290.unkD8;

        switch (temp_4_1289)
        {
          case 0:
            temp_17_1300 = &D_0013D290;
            sprintf(sp_slot, D_0013D270, *((s32 *) (((u8 *) (((u8 *) temp_17_1300) + (temp_17_1300->unkC4 * 0xB8))) + 0x14)));
            temp_3_1313 = temp_17_1300->unkC4 * 0xB8;
            if (sceMcOpen(*((s32 *) (((u8 *) temp_17_1300) + temp_3_1313)), *((s32 *) (((u8 *) (((u8 *) temp_17_1300) + temp_3_1313)) + 0x4)), sp_slot, 2) == 0)
          {
            temp_17_1300->unkD8 = 1;
            return;
          }
            break;

          case 1:
            var_5_269 = &D_0013D290;
            var_4_1020 = var_5_269->unkBC;
            if (var_4_1020 < 0)
          {
            var_5_269->unkE8 = (s32) var_5_269->unkC4;
            if (var_4_1020 == (-7))
            {
              var_5_269->unkE4 = 0x1E;
              goto block_297;
            }
            if (var_4_1020 == (-5))
            {
              var_5_269->unkE4 = 0x1F;
              goto block_297;
            }
            if (var_4_1020 == (-4))
            {
              var_5_269->unkE4 = 0x20;
              goto block_297;
            }
            if (var_4_1020 == (-3))
            {
              var_5_269->unkE4 = 0x21;
              goto block_297;
            }
            if (var_4_1020 == (-2))
            {
              var_5_269->unkE4 = 0x22;
              goto block_297;
            }
            var_5_269->unkE4 = 0x26;
            goto block_297;
          }
            goto block_214;
            goto block_352;

          case 2:
            var_16_1055 = &D_0013D290;
            var_2_1064 = sceMcSeek(var_16_1055->unkD0, 8, 0);
            goto block_300;

          case 3:
            var_3_1373 = &D_0013D290;
            if (var_3_1373->unkBC >= 0)
          {
            var_3_1373->unkD8 = 4;
          }
          else
          {
            block_326:
            var_3_1373->unkCC = 0;

            var_3_1373->unkD4 = 0;
            var_3_1373->unkC4 = 0;
          }
            break;

          case 4:
            temp_17_1385 = &D_0013D290;
            if (sceMcWrite(temp_17_1385->unkD0, D_0014EED0, GetDmaPacketSpanBytes(D_001A04C0)) == 0)
          {
            temp_17_1385->unkD8 = 5;
            return;
          }
            break;

          case 5:
            var_16_257 = &D_0013D290;
            temp_3_1400 = var_16_257->unkBC;
            if (temp_3_1400 == GetDmaPacketSpanBytes(D_001A04C0))
          {
            var_16_257->unkD8 = 6;
          }
          else
          {
            var_16_257->unkE8 = (s32) var_16_257->unkC4;
            if (temp_3_1400 >= 0)
            {
              var_2_690 = 0x1B;
              var_16_257->unkE4 = var_2_690;
              if (sceMcClose(var_16_257->unkD0) == 0)
              {
                var_16_257->unkCC = 0;
                var_16_257->unkD4 = 0;
                var_16_257->unkC4 = 0;
              }
            }
            else
            {
            if (temp_3_1400 == (-5))
            {
              var_16_257->unkE4 = 0x1F;
              goto block_148;
            }
            if (temp_3_1400 == (-4))
            {
              var_16_257->unkE4 = 0x23;
              goto block_148;
            }
            if (temp_3_1400 == (-3))
            {
              var_16_257->unkE4 = 0x24;
              goto block_148;
            }
            if (temp_3_1400 != (-2))
            {
              var_16_257->unkE4 = 0x26;
              goto block_148;
            }
            var_16_257->unkE4 = 0x22;
            goto block_148;
            }
          }
            break;

          case 6:
            var_16_1183 = &D_0013D290;
            if (var_16_1183->unkC8 == 0)
          {
            var_16_1183->unkD8 = 8;
          }
          else
          {
            var_2_1198 = sceMcSeek(var_16_1183->unkD0, var_16_1183->unkC8 * GetDmaPacketSpanBytes(D_001A07C0), 1);
            goto block_322;
          }
            break;

          case 7:
            var_3_1373 = &D_0013D290;
            if (var_3_1373->unkBC >= 0)
          {
            var_3_1373->unkD8 = 8;
          }
          else
          {
            goto block_326;
          }
            break;

          case 8:
            temp_17_1470 = &D_0013D290;
            if (sceMcWrite(temp_17_1470->unkD0, D_001506D0, GetDmaPacketSpanBytes(D_001A07C0)) == 0)
          {
            temp_17_1470->unkD8 = 9;
            return;
          }
            break;

          case 9:
            var_16_257 = &D_0013D290;
            temp_3_1485 = var_16_257->unkBC;
            if (temp_3_1485 == GetDmaPacketSpanBytes(D_001A07C0))
          {
            var_16_257->unkD8 = 0xA;
          }
          else
          {
            var_16_257->unkE8 = (s32) var_16_257->unkC4;
            if (temp_3_1485 >= 0)
            {
              var_2_690 = 0x25;
              var_16_257->unkE4 = var_2_690;
              if (sceMcClose(var_16_257->unkD0) == 0)
              {
                var_16_257->unkCC = 0;
                var_16_257->unkD4 = 0;
                var_16_257->unkC4 = 0;
                return;
              }
            }
            else
            {
              if (temp_3_1485 == (-5))
              {
                var_16_257->unkE4 = 0x1F;
              }
              else if (temp_3_1485 == (-4))
              {
                var_16_257->unkE4 = 0x23;
              }
              else if (temp_3_1485 == (-3))
              {
                var_16_257->unkE4 = 0x24;
              }
              else if (temp_3_1485 == (-2))
              {
                var_16_257->unkE4 = 0x18;
              }
              else
              {
                var_16_257->unkE4 = 0x1C;
              }

              goto block_148;
            }
          }
            break;

          case 10:
            var_16_123 = &D_0013D290;
            if (sceMcClose(var_16_123->unkD0) == 0)
          {
            goto block_346;
          }
            break;

        }

      }
      else
      {
        goto block_356;
      }
        break;

      case 17:
        func_001FD6E0(D_00137B80.unk14 << 0xB, &sp50, &sp54);
        func_00216788(sp50, D_00137B80.unk10, D_00137B80.unk14);
        var_3_504 = &D_0013D290;
        var_2_506 = 0x12;
        goto block_348;

      case 18:
        var_2_511 = &D_0013D290;
        if (*((s16 *) ((u8 *)0x1516D8)) == 0)
      {
        func_001FD6E0((*((s32 *) D_00137B94)) << 0xB, &sp58, &sp5C);
        temp_4_1566 = &D_0013D290;
        temp_4_1566->unkD8 = 0x13;
        temp_4_1566->unkEC = (void *) (((u32) sp58) + sp58->unk10);
      }
        block_351:
      var_2_511 = &D_0013D290;

        block_352:
      var_2_511->unkCC = 0;
      break;

      case 19:
        var_5_0 = (s32) (&D_0013D290);
        temp_4_1582 = (*((s32 *) (((u8 *) var_5_0) + 0xC4))) * 0xB8;
        var_2_955 = 0x2718;
        if ((*((s32 *) (((u8 *) (var_5_0 + temp_4_1582)) + 0xAC))) == 0)
      {
        if ((*((s32 *) (((u8 *) (var_5_0 + temp_4_1582)) + 0x14))) < 0)
        {
          var_2_955 = 0x27;
          goto block_356;
        }
        *((s32 *) (((u8 *) var_5_0) + 0xC8)) = 0;
        *((s32 *) (((u8 *) var_5_0) + 0xD8)) = 0;
        *((s32 *) (((u8 *) var_5_0) + 0xD4)) = 0x14;
        case 20:
          var_17_324 = &D_0013D290;

        temp_3_1605 = var_17_324->unkD8;
        switch (temp_3_1605)
        {
          case 0:
            sprintf(sp_slot, D_0013D270, *((s32 *) (((u8 *) (((u8 *) var_17_324) + (var_17_324->unkC4 * 0xB8))) + 0x14)));
            temp_3_1634 = var_17_324->unkC4 * 0xB8;
            if (sceMcOpen(*((s32 *) (((u8 *) var_17_324) + temp_3_1634)), *((s32 *) (((u8 *) (((u8 *) var_17_324) + temp_3_1634)) + 0x4)), sp_slot, 2) == 0)
          {
            var_17_324->unkD8 = 1;
          }
            break;

          case 1:
            temp_2_1645 = var_17_324->unkBC;
            if (temp_2_1645 >= 0)
          {
            var_17_324->unkD0 = temp_2_1645;
            temp_16_1652 = GetDmaPacketSpanBytes(D_001A04C0);
            temp_16_1660 = (temp_16_1652 + (GetDmaPacketSpanBytes(D_001A07C0) * 0x14)) + 8;
            var_17_324->unkF0 = temp_16_1660;
            if (sceMcWrite(var_17_324->unkD0, var_17_324->unkEC, temp_16_1660) == 0)
            {
              var_17_324->unkD8 = 2;
              return;
            }
          }
          else
          {
            var_3_590 = var_17_324->unkC4;
            var_2_676 = 0x2B;
            goto block_392;
          }
            break;

          case 2:
            temp_3_1674 = var_17_324->unkBC;
            if (temp_3_1674 == var_17_324->unkF0)
          {
            if (sceMcClose(var_17_324->unkD0) == 0)
            {
              var_17_324->unkD8 = 3;
              return;
            }
          }
          else
          {
            var_17_324->unkE8 = (s32) var_17_324->unkC4;
            if (temp_3_1674 >= 0)
            {
              var_17_324->unkE4 = 0xB;
              goto header_transfer_close;
            }
            if (temp_3_1674 == (-4))
            {
              var_17_324->unkE4 = 0x28;
              goto block_387;
            }
            if (temp_3_1674 == (-3))
            {
              var_17_324->unkE4 = 0x29;
              goto block_387;
            }
            if (temp_3_1674 == (-2))
            {
              var_17_324->unkE4 = 0x2A;
              goto block_387;
            }
            var_17_324->unkE4 = 0x2D;
            goto block_387;

          }
            break;

          case 3:
            var_2_676 = 0x2C;
            if (var_17_324->unkBC != 0)
          {
            var_17_324->unkE4 = var_2_676;
            var_17_324->unkE8 = (s32) var_17_324->unkC4;
          }
            var_17_324->unkCC = 0;
            var_17_324->unkD4 = 0;
            var_17_324->unkC4 = 0;
            break;

        }

      }
      else
      {
        goto block_356;
      }
        break;

    }

  }
}
#endif /* NON_MATCHING */
