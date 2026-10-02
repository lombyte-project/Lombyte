#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_002271d0/FUN_002271d0.s", FUN_002271d0);
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
struct DmaTag
{
  u32 w0;
  u32 addr;
  u32 w2;
  u32 w3;
};
struct TagPtr
{
  struct DmaTag *p;
};
struct ScreenInfo
{
  u8 pad0[0x150];
  s16 w;
  s16 h;
};
extern struct TagPtr D_00160F00;
extern struct ScreenInfo D_00151780;
extern void func_00233980(s32, s32);
void FUN_002271d0(void)
{
  struct DmaTag *tag;
  u64 *q;
  u64 *p;
  s32 w;
  s32 h;
  s32 n;
  s32 i;
  s32 x;
  s32 x0;
  s32 x1;
  u64 y0;
  u64 y1;

  w = D_00151780.w;
  h = D_00151780.h;
  n = w / 32;
  func_00233980(0x42, 0x64);
  D_00160F00.p->w0 = (n + 5) | 0x10000000;
  D_00160F00.p->addr = 0;
  D_00160F00.p->w2 = 0;
  D_00160F00.p->w3 = (n + 5) | 0x50000000;
  tag = D_00160F00.p;
  q = (u64 *)(tag + 1);
  D_00160F00.p = tag + 1;
  q[0] = 0x1000000000000001;
  q[1] = 0xE;
  q[2] = 0x31001;
  q[3] = 0x47;
  q[4] = 0x2400000000008001;
  q[5] = 0x10;
  q[6] = 0x146;
  q[7] = 0x7F808080;
  q[8] = 0x2400000000000000 | (n | 0x8000);
  q[9] = 0x44;
  i = 0;
  if (n > 0) {
    x = -(w * 8);
    y0 = (u64)(0x8000 - h * 8) << 16;
    x0 = x + 0x8000;
    p = q + 10;
    x1 = x + 0x8200;
    y1 = (u64)(h * 8 + 0x7FF0) << 16;
  loop:
    *p++ = x0 | y0;
    *p++ = x1 | y1;
    x1 += 0x200;
    x0 += 0x200;
    i++;
    if (i < n) {
      goto loop;
    }
  }
  do {
    D_00160F00.p = (struct DmaTag *)((u8 *)D_00160F00.p + 0x50 + n * 0x10);
  } while (0);
}
#endif /* NON_MATCHING */
