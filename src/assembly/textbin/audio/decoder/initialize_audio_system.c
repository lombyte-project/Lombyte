#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/audio/decoder/initialize_audio_system/FUN_0023a7c0.s", FUN_0023a7c0);
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
struct AudioState
{
  u8 pad0[0xD90F8];
  s32 dmac_handler_id;
  s32 intc_handler_id;
};
struct ThreadArgs
{
  u8 pad0[4];
  void (*entry)();
  void *stack;
  s32 stackSize;
  void *gp;
  s32 priority;
  u8 pad18[8];
  s32 gsTag;
  u8 pad24[0xC];
};
extern s32 D_00161208;
extern struct AudioState *D_0016120C;
extern s32 D_00161210;
extern u8 D_00166C00[];
extern u8 D_001E8AF0[];
extern void func_0023B5F0();
extern void func_0023B728();
extern void func_0023B3D8();



extern void func_0023B540() __asm__("FUN_0023b540");
extern void func_0023CE28();
extern s32 func_0023B940(struct AudioState *);
extern void sceMpegInit(void);
extern s32 func_0023CAC8(void *, u32, u32, void *, u32, u32, void *, u32);
extern s32 func_0023ABD0(void *, void *, u32, u32);
extern s32 func_0023CBD0(void *, u32, u32, void *, struct AudioState *);
extern void func_0023D190(void *, u32, u32, u32);
extern s32 CreateThread(struct ThreadArgs *);
extern s32 _StartThread(s32, void *);
extern s32 func_0023BA48(void *, s32, s32);
extern s32 DebugPrint(void *);
extern s32 AddIntcHandler(s32, void *, s32);
extern s32 func_00119090(s32);
extern s32 AddDmacHandler(s32, void *, s32);
extern s32 func_00119160(s32);
s32 initialize_audio_system(s32 arg0, s32 arg1, s32 arg2);
s32 initialize_audio_system(s32 arg0, s32 arg1, s32 arg2) __asm__("FUN_0023a7c0");

s32 initialize_audio_system(s32 arg0, s32 arg1, s32 arg2)
{
  struct ThreadArgs th;
  s32 tid;
  *((volatile s32 *) 0x1000E000) |= 3;
  *((volatile s32 *) 0x1000E010) = 4;
  func_0023B940(D_0016120C);
  sceMpegInit();
  func_0023CAC8(((u8 *) D_0016120C) + 0xD9048, D_00161208 + 0x1C85C0, 0xEB768, ((u8 *) D_0016120C) + 0x52040, D_00161208 + 0x1C7180, 0x100, ((u8 *) D_0016120C) + 0xD6040, 0x200);
  func_0023ABD0(((u8 *) D_0016120C) + 0xD9100, ((u8 *) D_0016120C) + 0x50040, 0x2000, D_00161208 + 0x1C8190);
  func_0023CBD0(((u8 *) D_0016120C) + 0xD9048, 0, 0, func_0023B5F0, D_0016120C);
  func_0023CBD0(((u8 *) D_0016120C) + 0xD9048, 3, arg2, func_0023B728, D_0016120C);
  func_0023D190(((u8 *) D_0016120C) + 0xD9168, (D_00161208 & 0x0FFFFFFF) | 0x20000000, D_00161208 + 0x1A0000, 2);
  th.stack = ((u8 *) D_0016120C) + 0xD2040;
  th.entry = func_0023CE28;
  th.stackSize = 0x4000;
  th.gp = D_00166C00;
  th.priority = 1;
  th.gsTag = 0;
  tid = CreateThread(&th);
  D_00161210 = tid;
  _StartThread(tid, ((u8 *) D_0016120C) + 0xD9048);
  if (func_0023BA48(((u8 *) D_0016120C) + 0xD9040, arg0, arg1) == 0)
  {
    if (arg1)
    {
      arg2 = 0;
      DebugPrint(D_001E8AF0);
    }
    else
    {
      arg2 = 0;
      DebugPrint(D_001E8AF0);
    }
  }
  else
  {
    arg2 = 1;
  }
  D_0016120C->intc_handler_id = AddIntcHandler(2, func_0023B3D8, 0);
  func_00119090(2);
  D_0016120C->dmac_handler_id = AddDmacHandler(2, func_0023B540, 0);
  func_00119160(2);
  return arg2;
}
#endif /* NON_MATCHING */
