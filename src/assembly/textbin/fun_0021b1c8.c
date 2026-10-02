#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021b1c8/FUN_0021b1c8.s", FUN_0021b1c8);
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
  s16 text;
  s16 enabled;
  s32 id;
  s16 subtext;
  s16 blink;
} MenuItem;
typedef struct 
{
  u8 pad0[0x20];
  s32 width;
  s32 height;
  u8 pad28[8];
  s32 flags;
  MenuItem *items;
  u8 pad38[8];
  s32 sel;
} Menu;
typedef struct 
{
  u8 pad0[0x40];
  Menu *focus;
} MenuState;
extern MenuState *D_001D5BF4[];
extern u8 D_001DF050[];
extern u8 D_001DF3F0[];
extern u8 D_001DF790[];



extern s32 D_001601B8 __attribute__((sda));



extern s32 D_001601BC __attribute__((sda));
extern void FUN_00233980(s32, long);
extern void func_001F4280(s32);
extern void func_001F4398(void);



extern s32 get_effect_texture(s32) __asm__("FUN_001f44b8");
extern void func_001F61E8(void);
extern void func_001F61F8(void);
extern s32 func_001F6200(char *, s32, u8 *);
extern void func_001F62B0(s32, s32, long, char *, s32, s32, u8 *);
extern char *func_001FDD10(s32);
extern s32 FUN_0021b6d8(s32, s32, s32);
s32 FUN_0021b1c8(Menu *menu)
{
  s32 focused;
  u8 *font;
  s32 kind;
  s32 rowh;
  s32 half;
  s32 maxw;
  s32 j;
  s32 size;
  s32 n;
  MenuItem *p;
  s32 i;
  s32 sel;
  s32 en;
  s32 color;
  char *text;
  char *sub;
  s32 x;
  s32 y;
  s32 w;
  s32 fl;
  s32 wd;
  MenuItem *it;
  s32 ty;
  s32 tx;
  size = 12;
  kind = 1;
  font = D_001DF050;
  focused = D_001D5BF4[0]->focus == menu;
  if (menu->flags & 4)
  {
    size = 14;
    kind = 3;
    font = D_001DF790;
  }
  if (menu->flags & 8)
  {
    size = 10;
    kind = 2;
    font = D_001DF3F0;
  }
  FUN_00233980(0x42, 0x44);
  FUN_00233980(0x47, 0x2004B);
  func_001F4280(0);
  n = 0;
  p = menu->items;
  while (p->text != 0)
  {
    p++;
    n++;
  }

  if (menu->flags & 0x10)
  {
    rowh = size + 3;
  }
  else
  {
    rowh = menu->height / (n + 1);
  }
  half = menu->width >> 1;
  maxw = 0;
  y = (rowh - (size / 2)) - 1;
  if (menu->flags & 0x4000)
  {
    if (menu->items[0].text != 0)
    {
      i = 0;
      j = 0;
      p = &menu->items[i];
      for (;;)
      {
        w = func_001F6200(func_001FDD10(p->text), -1, font);
        i++;
        j++;
        maxw = (maxw < w) ? (w) : (maxw);
        if (menu->items[j].text == 0)
        {
          break;
        }
        p = &menu->items[i];
      }

    }
  }
  fl = menu->flags;
  wd = menu->width;
  if (fl & 0x20000)
  {
    if (wd < (maxw + 6))
    {
      if (!(fl & 8))
      {
        menu->flags = fl | 8;
        return 1;
      }
    }
  }
  i = (j = 0);
  maxw = (wd < maxw) ? (wd) : (maxw);
  if (menu->items[0].text != 0)
  {
    do
    {
      sel = 0;
      if (focused)
      {
        sel = menu->sel == j;
      }
      it = &menu->items[i];
      en = it->enabled != 0;
      if (menu->flags & 2)
      {
        color = 0x80FFA888;
      }
      else
        if (sel)
      {
        if (en)
        {
          goto blink;
        }
        color = 0x80006060;
      }
      else
        if (en)
      {
        blink:
        color = FUN_0021b6d8(it->blink, -1, -1);

      }
      else
      {
        color = 0x80303030;
      }
      text = func_001FDD10(menu->items[i].text);
      if (menu->items[i].enabled == 2)
      {
        text = func_001FDD10(0x4F54);
      }
      x = half - (func_001F6200(text, -1, font) >> 1);
      if (menu->flags & 0x40)
      {
        x = 4;
      }
      else
        if (menu->flags & 0x4000)
      {
        x = half - (maxw >> 1);
      }
      func_001F61F8();
      tx = x + D_001601B8;
      ;
      func_001F62B0(tx, y + D_001601BC, 0x80000000L, text, -1, get_effect_texture(kind), font);
      func_001F61E8();
      if (menu->flags & 0x80)
      {
        func_001F61F8();
      }
      func_001F62B0(x, y, color, text, -1, get_effect_texture(kind), font);
      y += rowh;
      if (menu->items[i].subtext != 0)
      {
        func_001F61F8();
        tx = x + D_001601B8;
        ty = y + D_001601BC;
        sub = func_001FDD10(menu->items[i].subtext);
        func_001F62B0(tx, ty, 0x80000000L, sub, -1, get_effect_texture(kind), font);
        if (!(menu->flags & 0x80))
        {
          func_001F61E8();
        }
        sub = func_001FDD10(menu->items[i].subtext);
        func_001F62B0(x, y, color, sub, -1, get_effect_texture(kind), font);
        y += rowh;
      }
      if (menu->flags & 0x80)
      {
        func_001F61E8();
      }
      i++;
      j++;
    }
    while (menu->items[i].text != 0);
  }
  func_001F4398();
  return 2;
}
#endif /* NON_MATCHING */
