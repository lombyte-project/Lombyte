#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00203730/FUN_00203730.s", FUN_00203730);
#else

#include "types.h"
#include "qcopy.h"
#include "eetypes.h"

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
extern s32 registered_object_render_class_count __asm__("D_00160F4C");
extern s16 object_render_class_ids[] __asm__("D_001E1900");
extern u8 object_render_class_slot_by_id[] __asm__("D_001E1A00");
extern ObjectRenderClass *object_render_classes[] __asm__("D_001E1700");
extern s32 object_render_class_fixed_thresholds[] __asm__("D_001E2E00");
extern MaterialMap object_render_class_material_maps[] __asm__("D_001E3600");
extern s32 gs_texture_allocation_base __asm__("D_0015EE8C");
extern u64 resident_material_templates[] __asm__("D_0019E540");
extern s32 convert_float_to_word(f32) __asm__("func_001FA6D0");
extern s32 highest_set_bit_index(s32) __asm__("func_001F97A0");
void register_object_render_class(ObjectRenderClass *render_class, ResidentRenderTextureDefinition *textures, u128 *material_map, s32 class_id) __asm__("FUN_00203730");

void register_object_render_class(ObjectRenderClass *render_class, ResidentRenderTextureDefinition *textures, u128 *material_map, s32 class_id)
{
  s32 stream_index;
  s32 entry_index;
  s32 record_index;
  s32 material_index;
  s32 draw_high;
  s32 draw_shift;
  s32 material_base;
  s32 material_shift;
  s64 width_units_64;
  s64 width_units_128;
  s32 width_log2;
  s32 height_log2;
  s32 gs_block_base;
  s32 texture_block;
  s32 mip_block_0;
  s32 mip_block_1;
  s32 fixed_threshold;
  MaterialMap *slot_materials;
  ObjectRenderRecord *record;
  ResidentRenderTextureDefinition *texture;
  u64 *fallback_packet;
  u64 tex0_word;
  u64 tex1_word;
  u64 mip_word;
  u64 clamp_word;
  s32 packed_normals_offset;
  s32 records_offset;
  object_render_class_ids[registered_object_render_class_count] = class_id;
  object_render_class_slot_by_id[class_id] = registered_object_render_class_count;
  object_render_classes[registered_object_render_class_count] = render_class;
  render_class->class_slot = registered_object_render_class_count;
  fixed_threshold = convert_float_to_word(render_class->scale * 1024.0f);
  entry_index = registered_object_render_class_count++;
  object_render_class_fixed_thresholds[entry_index] = fixed_threshold;
  render_class->runtime_data = 0;
  render_class->runtime_index = 0;
  for (stream_index = 0; stream_index < 3; stream_index++)
  {
    if (render_class->streams[stream_index] != 0)
    {
      render_class->streams[stream_index] = (RenderStreamEntry *) (((s32) render_class->streams[stream_index]) + ((s32) render_class));
      for (entry_index = 0; entry_index < render_class->stream_counts[stream_index]; entry_index++)
      {
        render_class->streams[stream_index][entry_index].offset += (s32) render_class->streams[stream_index];
      }

    }
  }

  packed_normals_offset = render_class->packed_normals;
  records_offset = (s32) render_class->records;
  render_class->packed_normals = packed_normals_offset + (s32) render_class;
  render_class->records = (ObjectRenderRecord *) (records_offset + (s32) render_class);
  slot_materials = &object_render_class_material_maps[object_render_class_slot_by_id[class_id]];
  record = render_class->records;
  qcopy(slot_materials, material_map);
  for (record_index = 0; record_index < render_class->record_count; record_index++)
  {
    draw_high = record->tex1.w[0];
    material_index = slot_materials->b[record_index];
    draw_shift = record->tex1.w[1];
    material_base = record->clamp.w[0];
    material_shift = record->clamp.w[1];
    if (textures != 0)
    {
      texture = &textures[material_index];
      width_units_64 = ((s16) texture->width) >> 6;
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
      texture_block = texture->texture_block_offset + gs_block_base;
      mip_block_0 = texture->mip_block_offset_0 + gs_block_base;
      mip_block_1 = texture->mip_block_offset_1 + gs_block_base;
      tex0_word = (((u64) width_units_64) << 14) | ((((u64) width_log2) << 26) | 0x1300000);
      tex0_word |= ((u64) height_log2) << 30;
      tex0_word |= (((u64) texture_block) << 37) | (((u64) 1) << 34);
      tex0_word |= ((u64) 1) << 63;
      tex1_word = ((((u64) (texture->draw_control_count - 1)) << 2) | ((((u64) draw_shift) << 6) | 0x20)) | (((u64) draw_high) << 32);
      /* Keep the block address and enable bits above bit 31. */
      mip_word = ((((u64) width_units_128) << 14) | (((u64) mip_block_0) << 20)) | ((((u64) mip_block_1) << 40) | (((u64) 1) << 34));
      mip_word |= ((u64) 1) << 54;
      clamp_word = (material_base | (((u64) material_shift) << 2)) | (((u64) material_index) << 24);
      record->tex0 = tex0_word;
      record->tex1.d = tex1_word;
      record->mip = mip_word;
      record->clamp.d = clamp_word;
    }
    else
    {
      tex1_word = ((resident_material_templates[(material_index * 3) + 1] & 0x1C) | ((((u64) draw_shift) << 6) | 0x20)) | (((u64) draw_high) << 32);
      clamp_word = (material_base | (((u64) material_shift) << 2)) | (((u64) material_index) << 24);
      record->tex0 = resident_material_templates[material_index * 3];
      record->tex1.d = tex1_word;
      record->mip = resident_material_templates[(material_index * 3) + 2];
      record->clamp.d = clamp_word;
    }
    record->end = 0;
    record++;
  }

}

extern __typeof__(register_object_render_class) func_00203730 __attribute__((alias("FUN_00203730")));

#endif /* NON_MATCHING */
