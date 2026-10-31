/* Ported from rac1-decomp (src/game/loaders.c: func_00203F68). */
#include "types.h"
#include "qcopy.h"
#include "rnc/rendering/object_render_class.h"
#include "rnc/rendering/material_templates.h"
#include "rnc/rendering/render_globals.h"

extern s32 registered_object_render_class_count __asm__("D_00160F4C");
extern u8  object_render_class_slot_by_id[] __asm__("D_001E1A00");
extern s32 truncate_float_to_s32(f32) __asm__("func_001FA6D0");
extern s32 log2dim(s32) __asm__("func_001F97A0");

void register_object_render_class(ObjectRenderClass               *render_class,
                                  ResidentRenderTextureDefinition *textures,
                                  u128                            *material_map,
                                  s32 class_id) __asm__("FUN_00203730");

void register_object_render_class(ObjectRenderClass               *render_class,
                                  ResidentRenderTextureDefinition *textures,
                                  u128                            *material_map,

                                  s32 class_id) {
    s32                              slot;
    s32                              fixed_threshold;
    s32                              stream_index;
    s32                              record_index;
    s32                              packed_normals;
    ObjectRenderRecord              *records;
    ObjectRenderRecord              *render_record;
    MaterialMap                     *slot_materials;
    s32                              material_index;
    s32                              draw_high;
    s32                              material_base;
    s32                              material_shift;
    s32                              draw_shift;
    s32                              height_log2;
    s32                              width_bits;
    s32                              gs_block_base;
    s32                              mip_block_1;
    s32                              mip_block_0;
    s32                              draw_control_count;
    s32                              template_index;
    s32                              entry_index;
    u64                              tex1_word;
    u64                              clamp_word;
    u64                              template_tex1;
    u64                              template_tex0;
    u64                              template_mip;
    ResidentRenderTextureDefinition *texture;

    object_render_class_ids[registered_object_render_class_count] = class_id;
    object_render_class_slot_by_id[class_id] = registered_object_render_class_count;
    object_render_classes[registered_object_render_class_count] = render_class;
    render_class->class_slot = registered_object_render_class_count;
    fixed_threshold = truncate_float_to_s32(render_class->scale * 1024.0f);
    slot = registered_object_render_class_count;
    registered_object_render_class_count = slot + 1;
    object_render_class_fixed_thresholds[slot] = fixed_threshold;

    render_class->first_instance = 0;
    render_class->instance_count = 0;

    for (stream_index = 0; stream_index < 3; stream_index++) {
        if (render_class->streams[stream_index] != 0) {
            render_class->streams[stream_index] =
                (RenderStreamEntry *)((s32)render_class->streams[stream_index] + (s32)render_class);
            for (entry_index = 0; entry_index < render_class->stream_counts[stream_index];
                 entry_index++) {
                render_class->streams[stream_index][entry_index].offset +=
                    (s32)render_class->streams[stream_index];
            }
        }
    }

    packed_normals = render_class->packed_normals + (s32)render_class;
    records = (ObjectRenderRecord *)((s32)render_class->records + (s32)render_class);
    render_class->packed_normals = packed_normals;
    render_class->records = records;

    slot_materials = &object_render_class_material_maps[object_render_class_slot_by_id[class_id]];
    qcopy(slot_materials, material_map);

    for (record_index = 0, render_record = records; record_index < render_class->record_count;
         record_index++, render_record++) {
        draw_high = render_record->tex1.w[0];
        material_index = slot_materials->b[record_index];
        draw_shift = render_record->tex1.w[1];
        material_base = render_record->clamp.w[0];
        material_shift = render_record->clamp.w[1];

        if (textures != 0) {
            s32 width_units_64;
            s32 width_units_128;
            s32 width_log2;
            u64 tex0_word;
            u64 mip_word;

            texture = &textures[material_index];
            width_bits = (u16)texture->width << 16;
            width_units_64 = width_bits >> 22;
            width_units_128 = width_bits >> 23;
            if (width_units_64 <= 0) {
                width_units_64 = 1;
            }
            if (width_units_128 <= 0) {
                width_units_128 = 1;
            }
            width_log2 = log2dim(width_bits >> 16);
            height_log2 = log2dim(texture->height);
            gs_block_base = gs_texture_allocation_base >> 8;
            mip_block_1 = texture->mip_block_offset_1 + gs_block_base;
            mip_block_0 = texture->mip_block_offset_0 + gs_block_base;
            width_bits = texture->clut_block_offset + gs_block_base;
            draw_control_count = texture->draw_control_count;

            /* Fakematch: the signed/unsigned casts keep game cc1 from regrouping the ORs. */
            tex0_word = (s64)((u64)(((s64)width_units_64 << 14) | ((s64)0x13 << 20) |
                                    ((s64)width_log2 << 26)) |
                              ((u64)height_log2 << 30) | ((u64)1 << 34) | ((u64)width_bits << 37)) |
                        ((s64)4 << 61);
            tex1_word = (s64)(((u64)(draw_control_count - 1) << 2) | ((u64)1 << 5) |
                              ((u64)draw_shift << 6)) |
                        ((s64)draw_high << 32);
            mip_word = (s64)(((u64)width_units_128 << 14) | ((u64)mip_block_0 << 20) |
                             ((u64)1 << 34) | ((u64)mip_block_1 << 40)) |
                       ((s64)1 << 54);
            clamp_word =
                (u64)material_base | ((u64)material_shift << 2) | ((u64)material_index << 24);

            render_record->tex0.d = tex0_word;
            render_record->tex1.d = tex1_word;
            render_record->mip.d = mip_word;
            render_record->clamp.d = clamp_word;
            render_record->end = 0;
        } else {
            u64 draw_bits;
            u64 template_draw_word;
            u64 template_clamp_word;

            template_tex0 = resident_material_templates[material_index * 3];
            template_index = material_index * 3;
            template_tex1 = resident_material_templates[template_index + 1];
            template_mip = resident_material_templates[template_index + 2];
            draw_bits = ((u64)draw_shift << 6) | 0x20;
            template_draw_word = (template_tex1 & 0x1C) | draw_bits | ((u64)draw_high << 32);
            template_clamp_word =
                (u64)material_base | ((u64)material_shift << 2) | ((u64)material_index << 24);
            render_record->tex0.d = template_tex0;
            render_record->tex1.d = template_draw_word;
            render_record->mip.d = template_mip;
            render_record->clamp.d = template_clamp_word;
            render_record->end = 0;
        }
    }
}

ObjectRenderClass *object_render_classes[128] = {0};

s16 object_render_class_ids[128] = {0};

extern __typeof__(register_object_render_class) func_00203730
    __attribute__((alias("FUN_00203730")));
