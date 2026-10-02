#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00203730/FUN_00203730.s", FUN_00203730);
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
  u64 tex0;
  u64 pad8;
  union 
  {
    u64 d;
    s32 w[2];
  } tex1;
  u64 pad18;
  u64 mip;
  u64 pad28;
  union 
  {
    u64 d;
    s32 w[2];
  } clamp;
  u64 pad38;
  u64 end;
  u64 pad48;
} GsBlock;
typedef struct 
{
  s32 offset;
  u8 pad4[0xC];
} Reloc;
typedef struct 
{
  Reloc *relocs[3];
  s32 unkC;
  u8 pad10[0x10];
  u8 counts[3];
  u8 nblocks;
  u8 pad24[2];
  s16 unk26;
  s32 unk28;
  GsBlock *blocks;
  u8 pad30[0x16];
  u16 id;
  f32 scale;
} Model;
typedef union 
{
  u128 q;
  u8 b[16];
} Map16;
extern s32 D_00160F4C;
extern s16 D_001E1900[];
extern u8 D_001E1A00[];
extern Model *D_001E1700[];
extern s32 D_001E2E00[];
extern Map16 D_001E3600[];
extern s32 D_0015EE8C;
extern u64 D_0019E540[];
extern s32 func_001FA6D0(f32);
extern s32 func_001F97A0(s32);
void FUN_00203730(Model *model, TexEntry *tex, u128 *map, s32 cls)
{
  s32 i;
  int new_var;
  s32 j;
  s32 k;
  s32 m;
  s32 lo10;
  s32 hi10;
  s32 lo30;
  s32 hi30;
  s64 tw;
  s64 cw;
  u64 lw;
  u64 lh;
  s32 base;
  s32 r;
  Map16 *slot;
  GsBlock *blk;
  TexEntry *e;
  u64 *g;
  u64 d0;
  u64 d1;
  u64 d2;
  u64 d3;
  D_001E1900[D_00160F4C] = cls;
  D_001E1A00[cls] = D_00160F4C;
  D_001E1700[D_00160F4C] = model;
  model->id = D_00160F4C;
  r = func_001FA6D0(model->scale * 1024.0f);
  j = D_00160F4C++;
  D_001E2E00[j] = r;
  model->unk28 = 0;
  model->unk26 = 0;
  for (i = 0; i < 3; i++)
  {
    if (model->relocs[i] != 0)
    {
      model->relocs[i] = (Reloc *) (((s32) model->relocs[i]) + ((s32) model));
      for (j = 0; j < model->counts[i]; j++)
      {
        model->relocs[i][j].offset += (s32) model->relocs[i];
      }

    }
  }

  model->unkC += (s32) model;
  model->blocks = (GsBlock *) (((s32) model->blocks) + ((s32) model));
  slot = &D_001E3600[D_001E1A00[cls]];
  blk = model->blocks;
  slot->q = *map;
  for (k = 0; k < model->nblocks; k++)
  {
    lo10 = blk->tex1.w[0];
    m = slot->b[k];
    hi10 = blk->tex1.w[1];
    lo30 = blk->clamp.w[0];
    hi30 = blk->clamp.w[1];
    if (tex != 0)
    {
      e = &tex[m];
      tw = ((s16) e->size) >> 6;
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
      d0 = (((u64) tw) << 14) | ((((u64) lw) << 26) | 0x1300000);
      d0 |= ((u64) lh) << 30;
      d0 |= (((u64) (e->tbp + base)) << 37) | (((u64) 1) << 34);
      d0 |= ((u64) 1) << 63;
      d1 = ((((u64) (e->w - 1)) << 2) | ((((u64) hi10) << 6) | 0x20)) | (((u64) lo10) << 32);
      new_var = (((u64) (e->mbp + base)) << 40) | (((u64) 1) << 34);
      d2 = ((((u64) cw) << 14) | (((u64) (e->cbp + base)) << 20)) | new_var;
      d2 |= ((u64) 1) << 54;
      d3 = (lo30 | (((u64) hi30) << 2)) | (((u64) m) << 24);
      blk->tex0 = d0;
      blk->tex1.d = d1;
      blk->mip = d2;
      blk->clamp.d = d3;
    }
    else
    {
      d1 = ((D_0019E540[(m * 3) + 1] & 0x1C) | ((((u64) hi10) << 6) | 0x20)) | (((u64) lo10) << 32);
      d3 = (lo30 | (((u64) hi30) << 2)) | (((u64) m) << 24);
      blk->tex0 = D_0019E540[m * 3];
      blk->tex1.d = d1;
      blk->mip = D_0019E540[(m * 3) + 2];
      blk->clamp.d = d3;
    }
    blk->end = 0;
    blk++;
  }

}
#endif /* NON_MATCHING */
