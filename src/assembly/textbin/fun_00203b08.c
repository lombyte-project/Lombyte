#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00203b08/FUN_00203b08.s", FUN_00203b08);
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



typedef u32 qword[4] __attribute__((aligned(16)));



typedef s64 dword[2] __attribute__((aligned(16)));



typedef s32 s128 __attribute__((mode(TI), aligned(16)));



typedef u32 u128 __attribute__((mode(TI), aligned(16)));



typedef s32 sceVu0IVECTOR[4] __attribute__((aligned(16)));



typedef f32 sceVu0FVECTOR[4] __attribute__((aligned(16)));



typedef f32 sceVu0FMATRIX[4][4] __attribute__((aligned(16)));
typedef union 
{
  u128 ul128;
  u64 ul64[2];
  u32 ui32[4];
  f32 fl32[4];
  u16 us16[8];
  u8 uc8[16];
  sceVu0FVECTOR fv;
  sceVu0IVECTOR iv;
} Q_WORDDATA;
typedef struct 
{
  u8 pad0[4];
  u16 size;
  s16 h;
  s16 w;
  s16 tbp;
  s16 cbp;
  s16 mbp;
} TexEntry;
typedef struct 
{
  s16 size;
  s16 h;
  s16 w;
  s16 cbp;
  s16 tbp;
  s16 tbp1;
  s16 tbp2;
  s16 tbp3;
} EnvTex;
typedef struct 
{
  s32 w0;
  s32 w4;
  u8 pad8[8];
  s32 w10;
  s32 w14;
  u8 pad18[8];
  s32 w20;
  u8 pad24[0x1C];
} GsBlock;
typedef struct 
{
  u8 pad0[0x10];
  union 
  {
    u64 d;
    s32 w[2];
  } tex1;
  union 
  {
    u64 d;
    s32 w[2];
  } tex0;
  u64 miptbp1;
} EnvRegs;
typedef struct 
{
  s32 count;
  s32 skip;
} GroupHead;
typedef struct 
{
  u8 pad0[0x10];
  GroupHead head;
} Group;
typedef struct 
{
  Group *group;
  s32 pad4;
} GroupRef;
typedef struct 
{
  u8 pad0[0x10];
  f32 scale;
  u8 pad14[2];
  s16 unk16;
  s32 unk18;
  EnvRegs *env;
  u8 pad20[6];
  u16 id;
  s16 ngroups;
  u8 pad2A[2];
  s32 unk2C;
  u8 pad30[0x10];
  GroupRef groups[1];
} Model;
typedef union 
{
  u128 q;
  u8 b[16];
} Map16;
extern s32 D_001603CC;
extern u8 D_001D80B0[];
extern s16 D_001D8030[];
extern Model *D_001D7F30[];
extern s32 D_001D8CB0[];
extern Map16 D_001D92B0[];
extern s32 D_0015EE8C;
extern u64 D_0019E540[];
extern s32 func_001FA6D0(f32);
extern s32 func_001F97A0(s32);
void FUN_00203b08(Model *model, TexEntry *tex, u128 *map, EnvTex *envtex, s32 cls)
{
  s32 i;
  s32 j;
  s32 k;
  s32 m;
  s32 r;
  s32 tbw[4];
  s32 lo0;
  s32 hi0;
  s32 lo10;
  s32 hi10;
  s64 tw;
  s64 cw;
  u64 lw;
  u64 lh;
  s32 base;
  Map16 *slot;
  EnvRegs *env;
  GroupHead *head;
  GsBlock *blk;
  TexEntry *e;
  u64 d0;
  u64 d1;
  u64 d2;
  u64 d3;
  D_001D80B0[cls] = D_001603CC;
  D_001D8030[D_001603CC] = cls;
  D_001D7F30[D_001603CC] = model;
  model->id = D_001603CC;
  r = func_001FA6D0(model->scale * 1024.0f);
  j = D_001603CC++;
  D_001D8CB0[j] = r;
  model->unk18 = 0;
  model->unk16 = 0;
  if (model->unk2C != 0)
  {
    model->unk2C += (s32) model;
  }
  for (i = 0; i < model->ngroups; i++)
  {
    model->groups[i].group = (Group *) (((s32) model->groups[i].group) + ((s32) model));
  }

  if (model->env != 0)
  {
    env = (EnvRegs *) (((s32) model->env) + ((s32) model));
    model->env = env;
    lo10 = env->tex1.w[0];
    hi10 = env->tex1.w[1];
    if (envtex != 0)
    {
      for (i = 0; i < 4; i++)
      {
        tbw[i] = envtex->size >> (i + 6);
        if (tbw[i] <= 0)
        {
          tbw[i] = 1;
        }
      }

      lw = func_001F97A0(envtex->size);
      lh = func_001F97A0(envtex->h);
      env->tex1.d = ((((u64) (envtex->w - 1)) << 2) | ((((u64) hi10) << 6) | 0x20)) | (((u64) lo10) << 32);
      base = D_0015EE8C >> 8;
      env->tex0.d = ((((((u64) (envtex->tbp + base)) | (((u64) tbw[0]) << 14)) | ((((u64) lw) << 26) | 0x1300000)) | (((u64) lh) << 30)) | ((((u64) (envtex->cbp + base)) << 37) | (((u64) 1) << 34))) | (((u64) 1) << 63);
      env->miptbp1 = ((((((u64) (envtex->tbp1 + base)) | (((u64) tbw[1]) << 14)) | (((u64) (envtex->tbp2 + base)) << 20)) | (((u64) tbw[2]) << 34)) | (((u64) (envtex->tbp3 + base)) << 40)) | (((u64) tbw[3]) << 54);
    }
    else
    {
      for (k = 0; D_0019E540[(k + 1) * 3] != 0; k++)
      {
      }

      d1 = ((D_0019E540[(k * 3) + 1] & 0x1C) | ((((u64) hi10) << 6) | 0x20)) | (((u64) lo10) << 32);
      env->tex0.d = D_0019E540[k * 3];
      env->miptbp1 = D_0019E540[(k * 3) + 2];
      env->tex1.d = d1;
    }
  }
  slot = &D_001D92B0[D_001D80B0[cls]];
  slot->q = *map;
  for (i = 0; i < model->ngroups; i++)
  {
    head = &model->groups[i].group->head;
    blk = (GsBlock *) ((((u8 *) head) + (head->skip * 16)) + 0x10);
    for (j = 0; j < head->count; j++)
    {
      m = slot->b[blk->w20];
      lo0 = blk->w0;
      hi0 = blk->w4;
      lo10 = blk->w10;
      hi10 = blk->w14;
      if (tex != 0)
      {
        e = &tex[m];
        lo10 = ((s16) e->size) >> 6;
        tw = lo10;
        cw = ((s16) e->size) >> 7;
        if (tw <= 0)
        {
          tw = 1;
        }
        if (cw <= 0)
        {
          cw = 1;
        }
        lw = func_001F97A0((s16) e->size);
        lh = func_001F97A0(e->h);
        base = D_0015EE8C >> 8;
        d0 = ((((u64) (e->w - 1)) << 2) | ((((u64) hi0) << 6) | 0x20)) | (((u64) lo0) << 32);
        d1 = (lo10 | (((u64) hi10) << 2)) | (((u64) m) << 24);
        d2 = ((cw << 14) | (((u64) (e->cbp + base)) << 20)) | ((((u64) (e->mbp + base)) << 40) | (((u64) 1) << 34));
        d2 |= ((u64) 1) << 54;
        d3 = (((tw << 14) | ((lw << 26) | 0x1300000)) | (lh << 30)) | ((((u64) (e->tbp + base)) << 37) | (((u64) 1) << 34));
        d3 |= ((u64) 1) << 63;
        *((u64 *) (&blk->w0)) = d0;
        *((u64 *) (&blk->w10)) = d1;
        *((u64 *) (&blk->w20)) = d2;
        *((u64 *) (((u8 *) blk) + 0x30)) = d3;
      }
      else
      {
        d0 = ((D_0019E540[(m * 3) + 1] & 0x1C) | ((((u64) hi0) << 6) | 0x20)) | (((u64) lo0) << 32);
        d1 = (lo10 | (((u64) hi10) << 2)) | (((u64) m) << 24);
        *((u64 *) (&blk->w0)) = d0;
        *((u64 *) (&blk->w10)) = d1;
        *((u64 *) (&blk->w20)) = D_0019E540[(m * 3) + 2];
        *((u64 *) (((u8 *) blk) + 0x30)) = D_0019E540[m * 3];
      }
      blk++;
    }

  }

}
#endif /* NON_MATCHING */
