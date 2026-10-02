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
} ResidentRenderTextureDefinition;
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
} ObjectRenderRecord;
typedef struct 
{
  s32 offset;
  u8 pad4[0xC];
} RenderStreamEntry;
typedef struct 
{
  RenderStreamEntry *streams[3];
  s32 packed_normals;
  u8 pad10[0x10];
  u8 stream_counts[3];
  u8 record_count;
  u8 pad24[2];
  s16 runtime_index;
  s32 runtime_data;
  ObjectRenderRecord *records;
  u8 pad30[0x16];
  u16 class_slot;
  f32 scale;
} ObjectRenderClass;
typedef union 
{
  u128 q;
  u8 b[16];
} MaterialMap;
extern s32 D_00160F4C;
extern s16 D_001E1900[];
extern u8 D_001E1A00[];
extern ObjectRenderClass *D_001E1700[];
extern s32 D_001E2E00[];
extern MaterialMap D_001E3600[];
extern s32 D_0015EE8C;
extern u64 D_0019E540[];
extern s32 convert_float_to_word(f32) __asm__("func_001FA6D0");
extern s32 highest_set_bit_index(s32) __asm__("func_001F97A0");
void register_object_render_class(ObjectRenderClass *render_class, ResidentRenderTextureDefinition *textures, u128 *material_map, s32 class_id) __asm__("FUN_00203730");

void register_object_render_class(ObjectRenderClass *render_class, ResidentRenderTextureDefinition *textures, u128 *material_map, s32 class_id)
{
  s32 i;
  int mip_address_word;
  s32 j;
  s32 k;
  s32 material_index;
  s32 draw_high;
  s32 draw_shift;
  s32 material_base;
  s32 material_shift;
  s64 width_units_64;
  s64 width_units_128;
  u64 width_log2;
  u64 height_log2;
  s32 gs_block_base;
  s32 fixed_threshold;
  MaterialMap *slot_materials;
  ObjectRenderRecord *record;
  ResidentRenderTextureDefinition *texture;
  u64 *fallback_packet;
  u64 tex0_word;
  u64 tex1_word;
  u64 mip_word;
  u64 clamp_word;
  D_001E1900[D_00160F4C] = class_id;
  D_001E1A00[class_id] = D_00160F4C;
  D_001E1700[D_00160F4C] = render_class;
  render_class->class_slot = D_00160F4C;
  fixed_threshold = convert_float_to_word(render_class->scale * 1024.0f);
  j = D_00160F4C++;
  D_001E2E00[j] = fixed_threshold;
  render_class->runtime_data = 0;
  render_class->runtime_index = 0;
  for (i = 0; i < 3; i++)
  {
    if (render_class->streams[i] != 0)
    {
      render_class->streams[i] = (RenderStreamEntry *) (((s32) render_class->streams[i]) + ((s32) render_class));
      for (j = 0; j < render_class->stream_counts[i]; j++)
      {
        render_class->streams[i][j].offset += (s32) render_class->streams[i];
      }

    }
  }

  render_class->packed_normals += (s32) render_class;
  render_class->records = (ObjectRenderRecord *) (((s32) render_class->records) + ((s32) render_class));
  slot_materials = &D_001E3600[D_001E1A00[class_id]];
  record = render_class->records;
  slot_materials->q = *material_map;
  for (k = 0; k < render_class->record_count; k++)
  {
    draw_high = record->tex1.w[0];
    material_index = slot_materials->b[k];
    draw_shift = record->tex1.w[1];
    material_base = record->clamp.w[0];
    material_shift = record->clamp.w[1];
    if (textures != 0)
    {
      texture = &textures[material_index];
      width_units_64 = ((s16) texture->size) >> 6;
      width_units_128 = ((s16) texture->size) >> 7;
      if (width_units_64 <= 0)
      {
        width_units_64 = 1;
      }
      if (width_units_128 <= 0)
      {
        width_units_128 = 1;
      }
      width_log2 = highest_set_bit_index((s16) texture->size);
      height_log2 = highest_set_bit_index(texture->h);
      gs_block_base = D_0015EE8C >> 8;
      tex0_word = (((u64) width_units_64) << 14) | ((((u64) width_log2) << 26) | 0x1300000);
      tex0_word |= ((u64) height_log2) << 30;
      tex0_word |= (((u64) (texture->tbp + gs_block_base)) << 37) | (((u64) 1) << 34);
      tex0_word |= ((u64) 1) << 63;
      tex1_word = ((((u64) (texture->w - 1)) << 2) | ((((u64) draw_shift) << 6) | 0x20)) | (((u64) draw_high) << 32);
      mip_address_word = (((u64) (texture->mbp + gs_block_base)) << 40) | (((u64) 1) << 34);
      mip_word = ((((u64) width_units_128) << 14) | (((u64) (texture->cbp + gs_block_base)) << 20)) | mip_address_word;
      mip_word |= ((u64) 1) << 54;
      clamp_word = (material_base | (((u64) material_shift) << 2)) | (((u64) material_index) << 24);
      record->tex0 = tex0_word;
      record->tex1.d = tex1_word;
      record->mip = mip_word;
      record->clamp.d = clamp_word;
    }
    else
    {
      tex1_word = ((D_0019E540[(material_index * 3) + 1] & 0x1C) | ((((u64) draw_shift) << 6) | 0x20)) | (((u64) draw_high) << 32);
      clamp_word = (material_base | (((u64) material_shift) << 2)) | (((u64) material_index) << 24);
      record->tex0 = D_0019E540[material_index * 3];
      record->tex1.d = tex1_word;
      record->mip = D_0019E540[(material_index * 3) + 2];
      record->clamp.d = clamp_word;
    }
    record->end = 0;
    record++;
  }

}
#endif /* NON_MATCHING */
