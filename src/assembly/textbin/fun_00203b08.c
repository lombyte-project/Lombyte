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
} ResidentRenderTextureDefinition;
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
} ShrubMipTextureDefinition;
typedef struct 
{
  s32 draw_high;
  s32 draw_shift;
  u8 pad8[8];
  s32 material_base;
  s32 material_shift;
  u8 pad18[8];
  s32 material_index;
  u8 pad24[0x1C];
} ShrubRenderPacket;
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
} ShrubMipPacket;
typedef struct 
{
  s32 count;
  s32 packet_offset_quadwords;
} ShrubRenderGroupHeader;
typedef struct 
{
  u8 pad0[0x10];
  ShrubRenderGroupHeader group_header;
} ShrubRenderGroup;
typedef struct 
{
  ShrubRenderGroup *group;
  s32 pad4;
} ShrubRenderGroupReference;
typedef struct 
{
  u8 pad0[0x10];
  f32 scale;
  u8 pad14[2];
  s16 runtime_count;
  s32 runtime_data;
  ShrubMipPacket *mip_packet;
  u8 pad20[6];
  u16 class_slot;
  s16 group_count;
  u8 pad2A[2];
  s32 packed_geometry;
  u8 pad30[0x10];
  ShrubRenderGroupReference groups[1];
} ShrubRenderClass;
typedef union 
{
  u128 q;
  u8 b[16];
} MaterialMap;
extern s32 D_001603CC;
extern u8 D_001D80B0[];
extern s16 D_001D8030[];
extern ShrubRenderClass *D_001D7F30[];
extern s32 D_001D8CB0[];
extern MaterialMap D_001D92B0[];
extern s32 D_0015EE8C;
extern u64 D_0019E540[];
extern s32 convert_float_to_word(f32) __asm__("func_001FA6D0");
extern s32 highest_set_bit_index(s32) __asm__("func_001F97A0");
void register_shrub_render_class(ShrubRenderClass *render_class, ResidentRenderTextureDefinition *textures, u128 *material_map, ShrubMipTextureDefinition *mip_texture, s32 class_id) __asm__("FUN_00203b08");

void register_shrub_render_class(ShrubRenderClass *render_class, ResidentRenderTextureDefinition *textures, u128 *material_map, ShrubMipTextureDefinition *mip_texture, s32 class_id)
{
  s32 i;
  s32 j;
  s32 k;
  s32 material_index;
  s32 fixed_threshold;
  s32 mip_width_units[4];
  s32 draw_high;
  s32 draw_shift;
  s32 material_base;
  s32 material_shift;
  s64 width_units_64;
  s64 width_units_128;
  u64 width_log2;
  u64 height_log2;
  s32 gs_block_base;
  MaterialMap *slot_materials;
  ShrubMipPacket *mip_packet;
  ShrubRenderGroupHeader *group_header;
  ShrubRenderPacket *packet;
  ResidentRenderTextureDefinition *texture;
  u64 draw_word;
  u64 material_word;
  u64 mip_word;
  u64 texture_word;
  D_001D80B0[class_id] = D_001603CC;
  D_001D8030[D_001603CC] = class_id;
  D_001D7F30[D_001603CC] = render_class;
  render_class->class_slot = D_001603CC;
  fixed_threshold = convert_float_to_word(render_class->scale * 1024.0f);
  j = D_001603CC++;
  D_001D8CB0[j] = fixed_threshold;
  render_class->runtime_data = 0;
  render_class->runtime_count = 0;
  if (render_class->packed_geometry != 0)
  {
    render_class->packed_geometry += (s32) render_class;
  }
  for (i = 0; i < render_class->group_count; i++)
  {
    render_class->groups[i].group = (ShrubRenderGroup *) (((s32) render_class->groups[i].group) + ((s32) render_class));
  }

  if (render_class->mip_packet != 0)
  {
    mip_packet = (ShrubMipPacket *) (((s32) render_class->mip_packet) + ((s32) render_class));
    render_class->mip_packet = mip_packet;
    material_base = mip_packet->tex1.w[0];
    material_shift = mip_packet->tex1.w[1];
    if (mip_texture != 0)
    {
      for (i = 0; i < 4; i++)
      {
        mip_width_units[i] = mip_texture->size >> (i + 6);
        if (mip_width_units[i] <= 0)
        {
          mip_width_units[i] = 1;
        }
      }

      width_log2 = highest_set_bit_index(mip_texture->size);
      height_log2 = highest_set_bit_index(mip_texture->h);
      mip_packet->tex1.d = ((((u64) (mip_texture->w - 1)) << 2) | ((((u64) material_shift) << 6) | 0x20)) | (((u64) material_base) << 32);
      gs_block_base = D_0015EE8C >> 8;
      mip_packet->tex0.d = ((((((u64) (mip_texture->tbp + gs_block_base)) | (((u64) mip_width_units[0]) << 14)) | ((((u64) width_log2) << 26) | 0x1300000)) | (((u64) height_log2) << 30)) | ((((u64) (mip_texture->cbp + gs_block_base)) << 37) | (((u64) 1) << 34))) | (((u64) 1) << 63);
      mip_packet->miptbp1 = ((((((u64) (mip_texture->tbp1 + gs_block_base)) | (((u64) mip_width_units[1]) << 14)) | (((u64) (mip_texture->tbp2 + gs_block_base)) << 20)) | (((u64) mip_width_units[2]) << 34)) | (((u64) (mip_texture->tbp3 + gs_block_base)) << 40)) | (((u64) mip_width_units[3]) << 54);
    }
    else
    {
      for (k = 0; D_0019E540[(k + 1) * 3] != 0; k++)
      {
      }

      material_word = ((D_0019E540[(k * 3) + 1] & 0x1C) | ((((u64) material_shift) << 6) | 0x20)) | (((u64) material_base) << 32);
      mip_packet->tex0.d = D_0019E540[k * 3];
      mip_packet->miptbp1 = D_0019E540[(k * 3) + 2];
      mip_packet->tex1.d = material_word;
    }
  }
  slot_materials = &D_001D92B0[D_001D80B0[class_id]];
  slot_materials->q = *material_map;
  for (i = 0; i < render_class->group_count; i++)
  {
    group_header = &render_class->groups[i].group->group_header;
    packet = (ShrubRenderPacket *) ((((u8 *) group_header) + (group_header->packet_offset_quadwords * 16)) + 0x10);
    for (j = 0; j < group_header->count; j++)
    {
      material_index = slot_materials->b[packet->material_index];
      draw_high = packet->draw_high;
      draw_shift = packet->draw_shift;
      material_base = packet->material_base;
      material_shift = packet->material_shift;
      if (textures != 0)
      {
        texture = &textures[material_index];
        material_base = ((s16) texture->size) >> 6;
        width_units_64 = material_base;
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
        draw_word = ((((u64) (texture->w - 1)) << 2) | ((((u64) draw_shift) << 6) | 0x20)) | (((u64) draw_high) << 32);
        material_word = (material_base | (((u64) material_shift) << 2)) | (((u64) material_index) << 24);
        mip_word = ((width_units_128 << 14) | (((u64) (texture->cbp + gs_block_base)) << 20)) | ((((u64) (texture->mbp + gs_block_base)) << 40) | (((u64) 1) << 34));
        mip_word |= ((u64) 1) << 54;
        texture_word = (((width_units_64 << 14) | ((width_log2 << 26) | 0x1300000)) | (height_log2 << 30)) | ((((u64) (texture->tbp + gs_block_base)) << 37) | (((u64) 1) << 34));
        texture_word |= ((u64) 1) << 63;
        *((u64 *) (&packet->draw_high)) = draw_word;
        *((u64 *) (&packet->material_base)) = material_word;
        *((u64 *) (&packet->material_index)) = mip_word;
        *((u64 *) (((u8 *) packet) + 0x30)) = texture_word;
      }
      else
      {
        draw_word = ((D_0019E540[(material_index * 3) + 1] & 0x1C) | ((((u64) draw_shift) << 6) | 0x20)) | (((u64) draw_high) << 32);
        material_word = (material_base | (((u64) material_shift) << 2)) | (((u64) material_index) << 24);
        *((u64 *) (&packet->draw_high)) = draw_word;
        *((u64 *) (&packet->material_base)) = material_word;
        *((u64 *) (&packet->material_index)) = D_0019E540[(material_index * 3) + 2];
        *((u64 *) (((u8 *) packet) + 0x30)) = D_0019E540[material_index * 3];
      }
      packet++;
    }

  }

}
#endif /* NON_MATCHING */
