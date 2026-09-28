#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/runtime/memory/update_mode_freeze/FUN_001fce28.s", FUN_001fce28);
#else
#include "rnc/runtime_memory_update_mode_freeze_types.h"
extern struct S_0013C940 D_0013C940;
extern s32 D_0013CAE4[];
extern volatile u16 D_0013E05A[];
extern struct S_0013F350 D_0013F350;
extern u8 D_00141050[];
extern s32 D_0015ED84;
extern s32 D_0015EE38;
extern s32 D_0015EE3C;
extern s32 D_0015EEB0;
extern s32 D_0015EEB4;
extern s32 D_0015F5E8[];
extern s32 D_0015F604;
extern s32 D_0015F648;
extern u8 D_0016034C[];
extern struct S_00193300 D_00193300;
extern s32 D_0015EEB8[];
extern s32 D_001D5BF8[];
extern char func_0022CA50();
extern s32 func_001F96F8(s32);
extern void func_0020B178(s32, s32);
extern void func_001E93E8();
extern void func_001E9440(u8 *, u8 *, s32, s32);
extern void func_001F4A58(s32);
extern void func_001FF570(s32, s32);
extern void func_001FF768();
extern void func_00209370();
extern void func_0022E188(s32, s32);
extern void func_0012E418(s32);
extern void func_00216088();
extern void func_0012DC80();
void update_mode_freeze(void) __asm__("FUN_001fce28");

void update_mode_freeze(void)
{
  volatile s32 *new_var;
  register s32 new_var2 asm("a1");
  func_0022CA50();
  if (D_00193300.unk4 != 0)
  {
    D_00193300.unk4--;
  }
  switch (D_00193300.unk0)
  {
    case 5:
    {
      struct S_00193300 *st = &D_00193300;
      st->unk20++;
      if (func_001F96F8(0x5A) < st->unk20)
      {
        if (st->unk24 != 0)
        {
          do
          {
            st->unk24--;
          }
          while (0);
        }
      }
      if (func_001F96F8(0x78) < st->unk20)
      {
        if (D_0013CAE4[0] & 0x40)
        {
          D_0015F604 = 0;
          if (D_0015ED84 == 1)
          {
            func_0020B178(0, -1);
          }
        }
      }
      break;
    }

    case 4:
      if (D_0013CAE4[0] & 0x10)
    {
      struct S_0013F350_2080 *p = D_0013F350.unk2080;
      p->unk31 = 0;
      p->unk94 = 0;
      p->unk34 = (u16) (p->unk34 | 1);
      func_001E93E8();
      func_001E9440(((u8 *) (&D_0013F350)) + 0x1D00, ((u8 *) (&D_0013F350)) + 0x1D10, 0, 1);
      D_0015F604 = 0;
    }
    else
      if (D_0013CAE4[0] & 0x40)
    {
      D_0015F604 = 0;
    }
      break;

    case 6:
      switch (D_00193300.unk1C)
    {
      case 0:
        if (D_00193300.unk4 != 0)
      {
        if ((--D_00193300.unk4) == 0)
        {
          D_00193300.unk1C = 1;
        }
      }
      else
      {
        D_00193300.unk1C = 1;
      }
        break;

      case 1:
        if (D_0013CAE4[0] & 0x10)
      {
        D_0015F604 = D_00193300.unk14;
      }
      else
        if (D_0013CAE4[0] & 0x40)
      {
        func_001F4A58(4);
        ((u8 *) 0x16034C)[0] = 0;
        D_00193300.unk1C = 2;
        D_00193300.unk20 = func_001F96F8(0x258);
      }
        break;

      case 2:
        if ((D_00193300.unk20 == 0) || ((--D_00193300.unk20) == 0))
      {
        ((u8 *) 0x16034C)[0] = 1;
        D_00193300.unk1C = 3;
      }
      else
        if (D_0013CAE4[0] & 0x10)
      {
        func_001F4A58(4);
        ((u8 *) 0x16034C)[0] = 1;
        D_0015F604 = D_00193300.unk14;
      }
      else
        if (D_0013CAE4[0] & 0x40)
      {
        ((u8 *) 0x16034C)[0] = 0;
        D_0015F604 = D_00193300.unk14;
      }
        break;

      case 3:
        if (D_0013CAE4[0] & 0x40)
      {
        D_0015F604 = D_00193300.unk14;
      }
        break;

      default:
        D_0015F604 = D_00193300.unk14;
        break;

    }

      break;

    case 1:
      D_0015F648 = 2;
      if (D_0013CAE4[0] & 0x10)
    {
      ((s32 *) 0x15F5E8)[7] = 0;
      D_0013F350.unk160F = (u8) (D_0013F350.unk160F | 1);
    }
    else
      if (D_0013CAE4[0] & 0x40)
    {
      D_0015F604 = 0;
    }
      break;

    case 2:
      D_0015F648 = 2;
      if (D_0013CAE4[0] & 0x40)
    {
      D_0015F604 = 0;
    }
      break;

    case 0:
      switch (D_00193300.unk1C)
    {
      case 0:
        if (D_00193300.unk20 < 8)
      {
        D_00193300.unk20++;
      }
      else
        if (D_00193300.unk24 < 8)
      {
        D_00193300.unk24++;
      }
      else
      {
        D_00193300.unk1C = 1;
      }
        break;

      case 1:
        if (D_0013CAE4[0] & 0x40)
      {
        D_00193300.unk1C = 2;
      }
      else
        if (D_0013CAE4[0] & 0x820)
      {
        D_00193300.unk1C = 3;
      }
        break;

      case 2:

      case 3:
      {
        if (D_00193300.unk24 != 0)
        {
          D_00193300.unk24--;
        }
        else
          if (D_00193300.unk20 != 0)
        {
          D_00193300.unk20--;
        }
        else
        {
          new_var2 = D_00193300.unk1C;
          if (new_var2 == 2)
          {
            if (D_0013F350.unk880 != (-1))
            {
              func_001FF570(D_0013F350.unk880, 0);
              D_0013F350.unk880 = -1;
            }
            func_001FF768();
            if (func_001F96F8(0x1068) < D_0013F350.unk19C)
            {
              if (D_0015ED84 == 5)
              {
                D_0015EE38++;
              }
              else
                if (D_0015ED84 == 0x10)
              {
                D_0015EE3C++;
              }
            }
            D_0015F604 = 0;
            func_001E9440(D_00141050, D_00141050 + 0x10, 0, 1);
          }
          else
          {
            D_0015F604 = 0;
            if (D_0013F350.unk89A >= 3)
            {
              D_0013F350.unk89A = (s16) (((u16) D_0013F350.unk89A) - 1);
              D_0013F350.unk890->unkBC = 3;
            }
          }
        }
      }
        break;

      default:
        break;

    }

      break;

    case 3:
      D_00193300.unk20++;
      new_var = (volatile s32 *) 0x15EEB4;
      if (func_001F96F8(0x1E) < D_00193300.unk20)
    {
      if (D_00193300.unk24 != 0)
      {
        D_00193300.unk24--;
      }
    }
      switch (((s32 *) 0x15EEB8)[-2])
    {
      case 2:
        if (D_00193300.unk4 == 0)
      {
        if (D_0013CAE4[0] & 0x40)
        {
          D_0015EEB4 &= ~1;
          D_0015F604 = D_00193300.unk14;
        }
      }
        break;

      case 6:
        if (D_0013CAE4[0] & 0x20)
      {
        D_0015EEB4 |= 8;
      }
      else
        if (D_0013CAE4[0] & 0x10)
      {
        D_0015EEB4 = D_0015EEB4 | 0x20;
        D_0015EEB4 = D_0015EEB4 & (~2);
        D_0015EEB4 = D_0015EEB4 & (~4);
        if (((s32 *) 0x15F5E8)[0] == 0)
        {
          D_0015F604 = D_00193300.unk14;
        }
      }
        break;

      case 13:
        if (D_0013CAE4[0] & 0x20)
      {
        D_0015EEB4 |= 0x10;
      }
      else
        if (D_0013CAE4[0] & 0x10)
      {
        D_0015EEB4 = D_0015EEB4 | 0x20;
        D_0015EEB4 = D_0015EEB4 & (~2);
        D_0015EEB4 = D_0015EEB4 & (~4);
        if (((s32 *) 0x15F5E8)[0] == 0)
        {
          D_0015F604 = D_00193300.unk14;
        }
      }
        break;

      case 17:

      case 18:

      case 20:

      case 21:
        if (D_0013CAE4[0] & 0x40)
      {
        D_0015EEB4 = D_0015EEB4 ^ 0x40;
        D_0015EEB4 = D_0015EEB4 & (~2);
        D_0015EEB4 = D_0015EEB4 & (~4);
        D_0015F604 = D_00193300.unk14;
      }
        break;

      case 16:
        D_0015F604 = D_00193300.unk14;
        D_001D5BF8[0] = D_00193300.unk18;
        break;

      case 12:
        if (D_0015EEB4 & 2)
      {
        break;
      }

      case 3:

      case 5:
        if (D_0013CAE4[0] & 0x10)
      {
        D_0015EEB4 = D_0015EEB4 & (~2);
        D_0015EEB4 = D_0015EEB4 & (~4);
        D_0015F604 = D_00193300.unk14;
      }
        break;

      case 19:
        if ((((s32 *) 0x15F5E8)[0] != 0) && (D_0015EEB4 & 2))
      {
        if (D_0013CAE4[0] & 0x40)
        {
          func_00209370();
          D_0015EEB4 = D_0015EEB4 & (~2);
          D_0015EEB4 = D_0015EEB4 & (~4);
          D_0015EEB4 = D_0015EEB4 | 0x20;
          func_0022E188(0, -5);
          D_0013E05A[0] = 1;
        }
        else
          if (D_0013CAE4[0] & 0x10)
        {
          D_0015EEB4 = D_0015EEB4 & (~2);
          D_0015EEB4 = D_0015EEB4 & (~4);
          D_0015F604 = D_00193300.unk14;
        }
      }
      else
        if (D_0013CAE4[0] & 0x10)
      {
        D_0015EEB4 = D_0015EEB4 & (~2);
        D_0015EEB4 = D_0015EEB4 & (~4);
        D_0015F604 = D_00193300.unk14;
      }
        break;

      case 23:

      case 24:
        if (((s32 *) 0x13CAE4)[0] & 0x40)
      {
        func_00209370();
        *new_var = D_0015EEB4 & (~2);
        D_0015EEB4 = D_0015EEB4 & (~4);
        D_0015EEB4 = D_0015EEB4 | 0x20;
        func_0022E188(0, -5);
        D_0013E05A[0] = 1;
      }
      else
        if (D_0013CAE4[0] & 0x10)
      {
        D_0015EEB4 |= 0x20;
        D_0015F604 = D_00193300.unk14;
      }
        break;

      case 4:
        if (((s32 *) 0x15F5E8)[0] != 0)
      {
        if (D_0015EEB4 & 2)
        {
          struct S_0013C940 *p = &D_0013C940;
          if (p->unk1A4 & 0x40)
          {
            func_00209370();
            D_0015EEB4 = D_0015EEB4 & (~2);
            D_0015EEB4 = D_0015EEB4 & (~4);
            D_0015EEB4 = D_0015EEB4 | 0x20;
            func_0022E188(0, -5);
            D_0013E05A[0] = 1;
          }
          else
            if ((unsigned short) (p->unk1A4 & 0x10))
          {
            D_0015EEB4 = D_0015EEB4 | 0x20;
            D_0015EEB4 = D_0015EEB4 & (~2);
            D_0015EEB4 = D_0015EEB4 & (~4);
            D_0015F604 = D_00193300.unk14;
          }
        }
        else
          if (D_0015EEB4 & 4)
        {
          if (D_0013CAE4[0] & 0x10)
          {
            D_0015EEB4 = D_0015EEB4 | 0x20;
            D_0015EEB4 = D_0015EEB4 & (~2);
            D_0015EEB4 = D_0015EEB4 & (~4);
            D_0015F604 = D_00193300.unk14;
          }
        }
        else
        {
          D_0015F604 = D_00193300.unk14;
        }
      }
      else
        if (D_0013CAE4[0] & 0x10)
      {
        D_0015EEB4 = D_0015EEB4 | 0x20;
        D_0015EEB4 = D_0015EEB4 & (~2);
        D_0015EEB4 = D_0015EEB4 & (~4);
        D_0015F604 = D_00193300.unk14;
      }
        break;

      case 9:
        if (((s32 *) 0x15F5E8)[0] != 0)
      {
        if (!(D_0015EEB4 & 6))
        {
          D_0015F604 = D_00193300.unk14;
        }
      }
        break;

    }

      break;

  }

  if (((u32) (D_0015F604 - 3)) >= 2U)
  {
    func_0012E418(0x1D);
    func_00216088();
    func_0012DC80();
  }
}
#endif /* NON_MATCHING */
