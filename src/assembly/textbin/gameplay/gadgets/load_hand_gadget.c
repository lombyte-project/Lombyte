#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/gameplay/gadgets/load_hand_gadget/FUN_00224368.s", FUN_00224368);
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
typedef struct Moby
{
  u8 pad00[0x20];
  u8 state;
  u8 pad21[0x13];
  s16 update_kind;
  u8 pad36[0x1D];
  u8 anim;
  u8 pad54[0x20];
  void *update;
  void **vars;
  u8 pad7C[0x2A];
  s16 oclass;
  u8 padA8[0x14];
  u8 slot;
} Moby;
typedef struct Hand
{
  u8 pad00[0x44];
  s32 id;
  u8 pad48[4];
  Moby *m4C;
  Moby *m50;
  Moby *m54;
  Moby *m58;
  Moby *m5C;
  s32 x60;
  s32 x64;
  s32 x68;
  Moby *m6C;
  Moby *m70;
  Moby *m74;
  Moby *ammo[8];
  u8 pad98[0xC];
  u8 timers[0x18];
} Hand;
typedef struct GadgetRec
{
  u8 pad00[0x10];
  s32 oclass;
  u8 pad14[0x38];
} GadgetRec;
typedef struct HandState
{
  u8 pad00[0x1C];
  s32 cur;
  u8 pad20[0x10];
  s32 left;
  s32 x34;
  s32 x38;
  s32 x3C;
  u8 pad40[0x8C];
  s32 xCC;
  u8 padD0[0x48];
  s32 x118;
  s32 x11C;
  s32 x120;
  u8 pad124[0x1C];
  s32 x140;
  s32 x144;
} HandState;
typedef struct AnimGroup
{
  u8 pad0;
  u8 active;
  u8 pad2[0x1E];
  float x20;
  float x24;
  float x28;
} AnimGroup;
typedef struct SetRec
{
  s32 x0;
  s32 x4;
  s32 x8;
  s32 xC;
  s32 x10;
  s32 x14;
  s32 x18;
  s32 x1C;
  s32 x20;
  s32 x24;
  s32 x28;
  s32 x2C;
} SetRec;
extern u8 D_0013D4C0[];
extern u8 D_0013E520[];
typedef struct Game
{
  u8 pad0[0x10B8];
  s32 x10B8;
  u8 pad10BC[0xF3A];
  u8 x1FF6;
  u8 x1FF7;
} Game;
extern Game D_0013F350;
extern s32 D_0015FF50;
extern GadgetRec D_001863D0[];
extern u8 *D_001B3200[];
extern u8 D_001B3AC0[];
extern SetRec D_001D52E8[];
extern HandState D_001D5BF0;
extern AnimGroup D_001D5DD0;
extern AnimGroup D_001D5E10;
extern AnimGroup D_001D5E50;
extern float D_001D5E90[];
extern s32 D_001D5EA8[];
extern void func_001E9470(s32, s32);
extern void func_001E9478(Moby *, s32);
extern void select_world_object_resource_tables(s32, s32) __asm__("func_00204A40");
extern void func_0020CB10(s32, s32, AnimGroup *);
extern void func_0020CB88(s32, AnimGroup *);
extern void func_00212F90(void *, int, int, int);
extern Moby *func_00225490(s32);
extern Moby *func_00225530(Moby *);
extern void func_00225E70(s32, Moby *, Moby *, s32 *, s32 *, s32 *);
extern s32 queue_preview_animation(s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32) __asm__("func_002265D8");
extern s32 clear_preview_animation_queue(void) __asm__("func_00226718");
extern void func_00224B70();
extern void func_00224D28();
extern void func_00224E18();
extern void func_00224FC0();
extern void func_002250F0();
extern void func_00225180();
s32 load_hand_gadget(Hand *h) __asm__("FUN_00224368");

s32 load_hand_gadget(Hand *h)
{
  Moby *m;
  s32 cur0;
  s32 cur1;
  s32 cur2;
  s32 cur3;
  s32 cur4;
  s32 cur5;
  s32 cur6;
  s32 cls;
  s32 cls0;
  s32 idx;
  s32 gadget;
  s32 i;
  SetRec *rec;
  float *scale;
  s32 same;
  Moby **slot;
  gadget = 0;
  m = h->m54;
  cur0 = (m != 0) ? (m->oclass) : (-1);
  idx = D_001D5BF0.left;
  cls0 = D_001863D0[idx].oclass;
  same = cls0 == D_001D5BF0.x120;
  if ((cur0 != cls0) && same)
  {
    func_00225530(m);
    if (D_001D5DD0.active)
    {
      func_0020CB88(h->id, &D_001D5DD0);
    }
    if ((D_0013F350.x10B8 != 0) && (idx != D_0013F350.x10B8))
    {
      func_001E9470(0, 0);
    }
    D_0015FF50 = D_001D5BF0.x118 == 0;
    select_world_object_resource_tables(cls0, -1);
    D_001D5BF0.x11C = cls0;
    D_001D5BF0.x118 = D_0015FF50;
    D_001D5BF0.x140 = cls0;
    D_001D5BF0.x144 = D_0015FF50;
    D_001B3200[D_001B3AC0[cls0]][0xD] = 0;
    m = func_00225490(cls0);
    if (m != 0)
    {
      gadget = idx;
      if (D_0013E520[gadget])
      {
        func_001E9478(m, h->id);
      }
      *m->vars = h;
      m->update = func_00224B70;
      m->update_kind = 4;
      if (gadget == 0x12)
      {
        func_0020CB10(h->id, 0, &D_001D5DD0);
        D_001D5DD0.x20 = 0;
        D_001D5DD0.x24 = 0;
        D_001D5DD0.x28 = 0;
      }
    }
    h->m54 = m;
  }
  m = h->m50;
  cur1 = (m != 0) ? (m->oclass) : (-1);
  cls = D_001863D0[D_001D5BF0.x38].oclass;
  if (cur1 != cls)
  {
    m = func_00225530(m);
    if (cls != (-1))
    {
      m = func_00225490(cls);
      if (m != 0)
      {
        gadget = D_001D5BF0.x38;
        *m->vars = h;
        m->update = func_00224D28;
        m->update_kind = 4;
      }
    }
    h->m50 = m;
  }
  m = h->m58;
  cur2 = (m != 0) ? (m->oclass) : (-1);
  cls = D_001863D0[D_001D5BF0.x34].oclass;
  if (cur2 != cls)
  {
    m = func_00225530(m);
    if (D_001D5E10.active)
    {
      func_0020CB88(h->id, &D_001D5E10);
    }
    if (D_001D5E50.active)
    {
      func_0020CB88(h->id, &D_001D5E50);
    }
    if (cls != (-1))
    {
      m = func_00225490(cls);
      if (m != 0)
      {
        gadget = D_001D5BF0.x34;
        *m->vars = h;
        m->update = func_00224E18;
        m->update_kind = 4;
        func_0020CB10(h->id, 0x16, &D_001D5E10);
        func_0020CB10(h->id, 0x17, &D_001D5E50);
        D_001D5E50.x28 = (D_001D5E10.x28 = (D_001D5E50.x24 = (D_001D5E50.x20 = (D_001D5E10.x24 = (D_001D5E10.x20 = 0.01f)))));
      }
    }
    h->m58 = m;
    m = func_00225530(h->m5C);
    if (cls != (-1))
    {
      m = func_00225490(cls);
      if (m != 0)
      {
        *m->vars = h;
        m->update = func_00224E18;
        m->update_kind = 4;
      }
    }
    h->m5C = m;
  }
  m = h->m4C;
  cur3 = (m != 0) ? (m->oclass) : (-1);
  cls = D_001863D0[D_001D5BF0.x3C].oclass;
  if (cur3 != cls)
  {
    m = func_00225530(m);
    if (cls != (-1))
    {
      m = func_00225490(cls);
      if (m != 0)
      {
        *m->vars = h;
        m->update = func_00224FC0;
        m->update_kind = 4;
        if (m->oclass == 0x25F)
        {
          if (m->anim != 6)
          {
            func_00212F90(m, 6, 0, 10);
          }
          m->state = 8;
        }
      }
    }
    h->m4C = m;
  }
  m = h->m6C;
  cur4 = (m != 0) ? (m->oclass) : (-1);
  cls = (D_0013D4C0[0x23]) ? (0x197) : (-1);
  if (cur4 != cls)
  {
    m = func_00225530(m);
    if (cls != (-1))
    {
      m = func_00225490(cls);
      if (m != 0)
      {
        *m->vars = h;
        m->update = func_002250F0;
        m->update_kind = 4;
      }
    }
    h->m6C = m;
  }
  m = h->m70;
  cur5 = (m != 0) ? (m->oclass) : (-1);
  cls = (D_0013D4C0[0x21]) ? (0x266) : (-1);
  if (cur5 != cls)
  {
    m = func_00225530(m);
    if (cls != (-1))
    {
      m = func_00225490(cls);
      if (m != 0)
      {
        *m->vars = h;
        m->update = func_002250F0;
        m->update_kind = 4;
      }
    }
    h->m70 = m;
  }
  m = h->m74;
  cur6 = (m != 0) ? (m->oclass) : (-1);
  cls = (D_0013D4C0[0x22]) ? (0x26A) : (-1);
  if (cur6 != cls)
  {
    m = func_00225530(m);
    if (cls != (-1))
    {
      m = func_00225490(cls);
      if (m != 0)
      {
        *m->vars = h;
        m->update = func_002250F0;
        m->update_kind = 4;
      }
    }
    h->m74 = m;
  }
  i = 0;
  scale = D_001D5E90;
  slot = h->ammo;
  do
  {
    m = *slot;
    cur6 = (m != 0) ? (m->oclass) : (-1);
    cls = (i < D_0013F350.x1FF7) ? (0x1DF) : (-1);
    if (cur6 != cls)
    {
      m = func_00225530(m);
      if (cls != (-1))
      {
        m = func_00225490(cls);
        *scale = (i < D_0013F350.x1FF6) ? (0.0f) : (3.0f);
        D_001D5EA8[i] = 0;
        if (m != 0)
        {
          *m->vars = h;
          m->update = func_00225180;
          m->update_kind = 4;
          m->slot = i;
        }
      }
      i++;
      *slot = m;
      i++;
    }
    else
    {
    }
    slot++;
    scale++;
  }
  while (i < 8);
  if (((gadget != D_001D5BF0.cur) && (gadget > 0)) && (gadget < 0x24))
  {
    D_001D5BF0.cur = gadget;
    clear_preview_animation_queue();
    queue_preview_animation(D_001D52E8[gadget].x8 + D_001D5BF0.xCC, 0, D_001D52E8[gadget].xC, gadget, D_001D52E8[gadget].x10, D_001D52E8[gadget].x18, D_001D52E8[gadget].x1C, D_001D52E8[gadget].x20, D_001D52E8[gadget].x24, D_001D52E8[gadget].x28, D_001D52E8[gadget].x2C, D_001D52E8[gadget].x0, D_001D52E8[gadget].x4);
    if (D_001D52E8[gadget].xC != 0)
    {
      queue_preview_animation(D_001D52E8[gadget].x14 + D_001D5BF0.xCC, 2, 0, gadget, 1, -1, 0, -1, 0, -1, 0, 0, 0);
    }
  }
  for (i = 0; i < 0x18; i++)
  {
    if (h->timers[i] != 0)
    {
      h->timers[i]--;
    }
  }

  func_00225E70(h->id, h->m54, h->m50, &h->x60, &h->x64, &h->x68);
  return 0;
}
#endif /* NON_MATCHING */
