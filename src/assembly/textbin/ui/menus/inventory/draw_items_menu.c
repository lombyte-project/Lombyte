#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/ui/menus/inventory/draw_items_menu/FUN_0021eb20.s", FUN_0021eb20);
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
typedef struct 
{
  short s[12];
} TextBox;
typedef struct 
{
  u8 pad0[0x20];
  s32 w;
  u16 h;
  u8 pad26[0x1E];
  s32 tip;
} ItemsMenu;
struct Scr
{
  s32 pad[1];
  s32 x;
};
extern struct Scr D_0013E500[];



extern s32 D_0015ED88[] __attribute__((section(".sdata")));



extern s32 D_001601B8 __attribute__((sda));



extern s32 D_001601BC __attribute__((sda));
extern char D_001602A0[];
extern void func_001F4280(s32);
extern void func_001F4398(void);



extern void font_print_right(s32, s32, u64, char *, s32) __asm__("FUN_001f6940");



extern void font_print_left(s32, s32, u64, char *, s32) __asm__("FUN_001f6a60");
extern void func_001F7580(TextBox *, u64, char *, s32);
extern char *func_001FDD10(s32);
extern void func_00200E08(s32, s32, s32, s32, u64, s32);
extern void func_0020D330(s32, s32);
extern s32 func_00215248(void);
extern s32 func_00215290(void);
extern s32 func_00215300(void);
extern void *memset(void *, s32, u32);
extern int sprintf(char *, const char *, ...);
extern char *strchr(const char *, int);
extern char *strcpy(char *, const char *);
extern u32 strlen(const char *);
s32 draw_items_menu(ItemsMenu *m);
static inline int inline_fn(s32 arg0, int arg1)
{
  return arg0 + arg1;
}
static inline char *padd(char *a, int n)
{
  return a + n;
}
static inline int idiv(s32 a, int n)
{
  return a / n;
}

s32 draw_items_menu(ItemsMenu *m) __asm__("FUN_0021eb20");

s32 draw_items_menu(ItemsMenu *m)
{
  char buf[0x80];
  char *p;
  char *q;
  char *r;
  char *fmt;
  s32 len;
  s32 k;
  s32 x;
  s32 y;
  s32 new_var;
  if (m->tip != 0)
  {
    func_0020D330(m->tip, 1);
  }
  func_001F4280(0);
  k = 3;
  {
    TextBox c = {{0, m->h, 8, m->w / k, 0, 0, 0, 0, 0x10, 5}};
    c.s[4] = inline_fn(c.s[2], c.s[3]) >> 1;
    strcpy(buf, func_001FDD10(0x4F4E));
    if (D_0015ED88[0] == k)
    {
      p = strchr(buf, 0x2D);
      if (p != 0)
      {
        len = strlen(buf);
        q = buf + len;
        if (p < q)
        {
          r = q;
          do
          {
            r[1] = *q;
            r--;
            q = r;
            len--;
          }
          while (p < q);
        }
        buf[inline_fn(len, 1)] = 0x20;
      }
    }
    func_001F7580(&c, 0x8000C0C0L, buf, -1);
    c.s[9] ^= 4;
    c.s[5] = (D_0013E500[0].x - c.s[7]) >> 1;
    func_001F7580(&c, 0x8000C0C0L, buf, -1);
  }
  x = inline_fn(D_001601B8, 0xC8);
  y = inline_fn(D_001601BC, 0x1D);
  font_print_right(x, y, 0x80000000L, func_001FDD10(0x4F4F), -1);
  x = inline_fn(D_001601B8, 0xC8);
  y = inline_fn(D_001601BC, 0x36);
  font_print_right(x, y, 0x80000000L, func_001FDD10(0x4F50), -1);
  x = inline_fn(D_001601B8, 0xC8);
  y = inline_fn(D_001601BC, 0x54);
  font_print_right(x, y, 0x80000000L, func_001FDD10(0x4F51), -1);
  font_print_right(0xC8, 0x1D, 0x80FFA888L, func_001FDD10(0x4F4F), -1);
  font_print_right(0xC8, 0x36, 0x80FFA888L, func_001FDD10(0x4F50), -1);
  font_print_right(0xC8, 0x54, 0x80FFA888L, func_001FDD10(0x4F51), -1);
  new_var = func_00215290();
  sprintf(buf, D_001602A0, new_var);
  font_print_left(inline_fn(D_001601B8, 0xF0), inline_fn(D_001601BC, 0x1D), 0x80000000L, buf, -1);
  font_print_left(0xF0, 0x1D, 0x80FFA888L, buf, -1);
  sprintf(buf, D_001602A0, func_00215300() * 4);
  font_print_left(inline_fn(D_001601B8, 0xF0), inline_fn(D_001601BC, 0x36), 0x80000000L, buf, -1);
  font_print_left(0xF0, 0x36, 0x80FFA888L, buf, -1);
  sprintf(buf, D_001602A0, func_00215248());
  font_print_left(inline_fn(D_001601B8, 0xF0), inline_fn(D_001601BC, 0x54), 0x80000000L, buf, -1);
  font_print_left(0xF0, 0x54, 0x80FFA888L, buf, -1);
  func_00200E08(inline_fn(D_001601B8, 0xD0), inline_fn(D_001601BC, 0x4D), inline_fn(D_001601B8, 0xF2), inline_fn(D_001601BC, 0x50), 0x80000000L, 0);
  func_00200E08(0xD0, 0x4D, 0xF2, 0x50, 0x80FFA888L, 0);
  func_001F4398();
  return 8;
}
#endif /* NON_MATCHING */
