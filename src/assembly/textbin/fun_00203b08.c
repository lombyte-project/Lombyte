#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00203b08/FUN_00203b08.s", FUN_00203b08);
#else

#include "eetypes.h"
#include "qcopy.h"

typedef struct 
{
  u8 pad0[4];
  u16 width;
  s16 height;
  s16 draw_control_count;
  s16 texture_block_offset;
  s16 mip_block_offset_0;
  s16 mip_block_offset_1;
} ResidentRenderTextureDefinition;
typedef struct 
{
  s16 width;
  s16 height;
  s16 draw_control_count;
  s16 texture_block_offset;
  s16 palette_block_offset;
  s16 mip_block_offset_0;
  s16 mip_block_offset_1;
  s16 mip_block_offset_2;
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
  u64 payload18;
  union 
  {
    u64 d;
    s32 w[2];
  } tex0;
  u64 payload28;
  u64 miptbp1;
  u64 payload38;
} ShrubMipPacket;
typedef struct 
{
  s32 count;
  s32 prefix_record_count;
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
extern s32 registered_shrub_render_class_count __asm__("D_001603CC");
extern u8 shrub_render_class_slot_by_id[] __asm__("D_001D80B0");
extern s16 shrub_render_class_ids[] __asm__("D_001D8030");
extern ShrubRenderClass *shrub_render_classes[] __asm__("D_001D7F30");
extern s32 shrub_render_class_fixed_thresholds[] __asm__("D_001D8CB0");
extern MaterialMap shrub_render_class_material_maps[] __asm__("D_001D92B0");
extern s32 gs_texture_allocation_base __asm__("D_0015EE8C");
extern u64 resident_material_templates[] __asm__("D_0019E540");
extern s32 convert_float_to_word(f32) __asm__("func_001FA6D0");
extern s32 highest_set_bit_index(s32) __asm__("func_001F97A0");
void register_shrub_render_class(ShrubRenderClass *render_class, ResidentRenderTextureDefinition *textures, u128 *material_map, ShrubMipTextureDefinition *mip_texture, s32 class_id) __asm__("FUN_00203b08");

void register_shrub_render_class(ShrubRenderClass *render_class, ResidentRenderTextureDefinition *textures, u128 *material_map, ShrubMipTextureDefinition *mip_texture, s32 class_id)
{
  s32 group_index;
  s32 index;
  s32 template_index;
  s32 material_index;
  s32 packet_material_base;
  s32 fixed_threshold;
  s32 mip_width_units[4];
  s32 draw_high;
  s32 draw_shift;
  s32 material_base;
  s32 material_shift;
  s32 width_units_64;
  s32 width_units_128;
  s32 width_log2;
  s32 height_log2;
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
  shrub_render_class_slot_by_id[class_id] = registered_shrub_render_class_count;
  shrub_render_class_ids[registered_shrub_render_class_count] = class_id;
  shrub_render_classes[registered_shrub_render_class_count] = render_class;
  render_class->class_slot = registered_shrub_render_class_count;
  fixed_threshold = convert_float_to_word(render_class->scale * 1024.0f);
  index = registered_shrub_render_class_count++;
  shrub_render_class_fixed_thresholds[index] = fixed_threshold;
  render_class->runtime_data = 0;
  render_class->runtime_count = 0;
  if (render_class->packed_geometry != 0)
  {
    render_class->packed_geometry += (s32) render_class;
  }
  for (group_index = 0; group_index < render_class->group_count; group_index++)
  {
    render_class->groups[group_index].group = (ShrubRenderGroup *) (((s32) render_class->groups[group_index].group) + ((s32) render_class));
  }

  if (render_class->mip_packet != 0)
  {
    mip_packet = (ShrubMipPacket *) (((s32) render_class->mip_packet) + ((s32) render_class));
    render_class->mip_packet = mip_packet;
    material_base = mip_packet->tex1.w[0];
    material_shift = mip_packet->tex1.w[1];
    if (mip_texture != 0)
    {
      for (group_index = 0; group_index < 4; group_index++)
      {
        mip_width_units[group_index] = mip_texture->width >> (group_index + 6);
        if (mip_width_units[group_index] <= 0)
        {
          mip_width_units[group_index] = 1;
        }
      }

      width_log2 = highest_set_bit_index(mip_texture->width);
      height_log2 = highest_set_bit_index(mip_texture->height);
      mip_packet->tex1.d = ((((u64) (mip_texture->draw_control_count - 1)) << 2) | ((((u64) material_shift) << 6) | 0x20)) | (((u64) material_base) << 32);
      gs_block_base = gs_texture_allocation_base >> 8;
      mip_packet->tex0.d = ((((((u64) (mip_texture->palette_block_offset + gs_block_base)) | (((u64) mip_width_units[0]) << 14)) | ((((u64) width_log2) << 26) | 0x1300000)) | (((u64) height_log2) << 30)) | ((((u64) (mip_texture->texture_block_offset + gs_block_base)) << 37) | (((u64) 1) << 34))) | (((u64) 1) << 63);
      mip_packet->miptbp1 = ((((((u64) (mip_texture->mip_block_offset_0 + gs_block_base)) | (((u64) mip_width_units[1]) << 14)) | (((u64) (mip_texture->mip_block_offset_1 + gs_block_base)) << 20)) | (((u64) mip_width_units[2]) << 34)) | (((u64) (mip_texture->mip_block_offset_2 + gs_block_base)) << 40)) | (((u64) mip_width_units[3]) << 54);
    }
    else
    {
      for (template_index = 0; resident_material_templates[(template_index + 1) * 3] != 0; template_index++)
      {
      }

      material_word = ((resident_material_templates[(template_index * 3) + 1] & 0x1C) | ((((u64) material_shift) << 6) | 0x20)) | (((u64) material_base) << 32);
      mip_packet->tex0.d = resident_material_templates[template_index * 3];
      mip_packet->miptbp1 = resident_material_templates[(template_index * 3) + 2];
      mip_packet->tex1.d = material_word;
    }
  }
  /* Copy all sixteen selectors, as one quadword, before rebuilding packets. */
  slot_materials = &shrub_render_class_material_maps[shrub_render_class_slot_by_id[class_id]];
  qcopy(slot_materials, material_map);
  for (group_index = 0; group_index < render_class->group_count; group_index++)
  {
    group_header = &render_class->groups[group_index].group->group_header;
    packet = (ShrubRenderPacket *) ((((u8 *) group_header) + (group_header->prefix_record_count * 16)) + 0x10);
    for (index = 0; index < group_header->count; index++)
    {
      material_index = slot_materials->b[packet->material_index];
      draw_high = packet->draw_high;
      draw_shift = packet->draw_shift;
      material_base = packet->material_base;
      /* The original material control survives the width scratch below. */
      packet_material_base = material_base;
      material_shift = packet->material_shift;
      if (textures != 0)
      {
        texture = &textures[material_index];
        material_base = ((s16) texture->width) >> 6;
        width_units_64 = material_base;
        width_units_128 = ((s16) texture->width) >> 7;
        if (width_units_64 <= 0)
        {
          width_units_64 = 1;
        }
        if (width_units_128 <= 0)
        {
          width_units_128 = 1;
        }
        width_log2 = highest_set_bit_index((s16) texture->width);
        height_log2 = highest_set_bit_index(texture->height);
        gs_block_base = gs_texture_allocation_base >> 8;
        draw_word = ((((u64) (texture->draw_control_count - 1)) << 2) | ((((u64) draw_shift) << 6) | 0x20)) | (((u64) draw_high) << 32);
        material_word = (packet_material_base | (((u64) material_shift) << 2)) | (((u64) material_index) << 24);
        mip_word = (((u64) width_units_128 << 14) | (((u64) (texture->mip_block_offset_0 + gs_block_base)) << 20)) | ((((u64) (texture->mip_block_offset_1 + gs_block_base)) << 40) | (((u64) 1) << 34));
        mip_word |= ((u64) 1) << 54;
        texture_word = ((((u64) width_units_64 << 14) | (((u64) width_log2 << 26) | 0x1300000)) | ((u64) height_log2 << 30)) | ((((u64) (texture->texture_block_offset + gs_block_base)) << 37) | (((u64) 1) << 34));
        texture_word |= ((u64) 1) << 63;
        *((u64 *) (&packet->draw_high)) = draw_word;
        *((u64 *) (&packet->material_base)) = material_word;
        *((u64 *) (&packet->material_index)) = mip_word;
        *((u64 *) (((u8 *) packet) + 0x30)) = texture_word;
      }
      else
      {
        draw_word = ((resident_material_templates[(material_index * 3) + 1] & 0x1C) | ((((u64) draw_shift) << 6) | 0x20)) | (((u64) draw_high) << 32);
        material_word = (packet_material_base | (((u64) material_shift) << 2)) | (((u64) material_index) << 24);
        *((u64 *) (&packet->draw_high)) = draw_word;
        *((u64 *) (&packet->material_base)) = material_word;
        *((u64 *) (&packet->material_index)) = resident_material_templates[(material_index * 3) + 2];
        *((u64 *) (((u8 *) packet) + 0x30)) = resident_material_templates[material_index * 3];
      }
      packet++;
    }

  }

}

extern __typeof__(register_shrub_render_class) func_00203B08 __attribute__((alias("FUN_00203b08")));

#endif /* NON_MATCHING */
