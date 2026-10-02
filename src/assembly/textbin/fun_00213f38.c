#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00213f38/FUN_00213f38.s", FUN_00213f38);
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
extern f32 func_001F99C0(f32);
extern f32 func_001F9988(f32);
extern void func_00213ED8(f32 *, f32, f32);
f32 FUN_00213f38(f32 *arg0, f32 *arg1, f32 fparg0, f32 fparg1, f32 fparg2, f32 fparg3)
{
  f32 d;
  f32 h;
  f32 s;
  f32 a;
  f32 b;
  f32 v;
  d = fparg0 - (*arg0);
  if ((((*arg1) * d) >= 0.0f) && (d != 0.0f))
  {
    h = (((*arg1) * (*arg1)) / fparg2) * 0.5f;
    if (func_001F99C0(d) < h)
    {
      s = func_001F99C0(d);
      s = s + func_001F99C0(*arg1);
      if (h < s)
      {
        a = 0.0f;
        b = fparg2;
        func_00213ED8(arg1, a, b);
      }
      else
      {
        func_00213ED8(arg1, 0.0f, fparg2 * 1.1f);
        goto tail;
      }
    }
    else
    {
      v = func_001F9988((2.0f * fparg2) * d);
      if (fparg3 < v)
      {
        v = fparg3;
      }
      if (d < 0.0f)
      {
        a = -v;
        b = fparg1;
        func_00213ED8(arg1, a, b);
      }
      else
      {
        func_00213ED8(arg1, v, fparg1);
      }
    }
    tail:
    s = func_001F99C0(d);

    if (func_001F99C0(*arg1) < s)
    {
      *arg0 = (*arg0) + (*arg1);
      return *arg1;
    }
    *arg0 = fparg0;
    return d;
  }
  func_00213ED8(arg1, 0.0f, fparg2);
  s = (*arg0) + (*arg1);
  *arg0 = s;
  return *arg1;
}
#endif /* NON_MATCHING */
