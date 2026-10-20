#include "types.h"
#include "eetypes.h"
#include "rnc/rendering/shrub_render_class.h"
#include "qcopy.h"

extern s32 registered_shrub_render_class_count __asm__("D_001603CC");
#include "rnc/rendering/render_globals.h"
#include "rnc/rendering/material_templates.h"
extern s32 convert_float_to_word(f32) __asm__("func_001FA6D0");
extern s32 highest_set_bit_index(s32) __asm__("func_001F97A0");
void register_shrub_render_class(ShrubRenderClass *render_class,
                                 ResidentRenderTextureDefinition *textures, u128 *material_map,
                                 ShrubMipTextureDefinition *mip_texture,
                                 s32 class_id) __asm__("FUN_00203b08");

void register_shrub_render_class(ShrubRenderClass *render_class,
                                 ResidentRenderTextureDefinition *textures, u128 *material_map,
                                 ShrubMipTextureDefinition *mip_texture, s32 class_id) {
    s32 group_index;
    s32 index;
    s32 fixed_threshold;
    MaterialMap *slot_materials;
    ShrubMipPacket *packed_mip_packet;
    ShrubMipPacket *mip_packet;
    ShrubRenderGroupHeader *group_header;
    ShrubRenderPacket *packet;

    shrub_render_class_slot_by_id[class_id] = registered_shrub_render_class_count;
    shrub_render_class_ids[registered_shrub_render_class_count] = class_id;
    shrub_render_classes[registered_shrub_render_class_count] = render_class;
    render_class->class_slot = registered_shrub_render_class_count;
    fixed_threshold = convert_float_to_word(render_class->scale * 1024.0f);
    shrub_render_class_fixed_thresholds[registered_shrub_render_class_count] = fixed_threshold;
    registered_shrub_render_class_count++;
    render_class->first_instance = 0;
    render_class->instance_count = 0;
    if (render_class->packed_geometry != 0) {
        render_class->packed_geometry += (s32)render_class;
    }
    for (group_index = 0; group_index < render_class->group_count; group_index++) {
        render_class->groups[group_index].group =
            (ShrubRenderGroup *)(((s32)render_class->groups[group_index].group) +
                                 ((s32)render_class));
    }

    packed_mip_packet = render_class->mip_packet;
    if (packed_mip_packet != 0) {
        s32 material_base;
        s32 material_shift;

        mip_packet = (ShrubMipPacket *)((s32)packed_mip_packet + (s32)render_class);
        render_class->mip_packet = mip_packet;
        material_base = mip_packet->tex1.w[0];
        material_shift = mip_packet->tex1.w[1];
        if (mip_texture != 0) {
            s32 mip_width_units[4];
            s32 width_log2;
            s32 height_log2;
            s32 gs_block_base;
            u64 draw_word;
            s32 width;
            s32 level;
            s32 draw_control_count;
            s32 palette_block;
            s32 texture_block;
            s32 mip_block_0;
            s32 mip_block_1;
            s32 mip_block_2;

            width = mip_texture->width;
            for (level = 0; level < 4; level++) {
                mip_width_units[level] = width >> (level + 6);
                if (mip_width_units[level] <= 0) {
                    mip_width_units[level] = 1;
                }
            }

            width_log2 = highest_set_bit_index(mip_texture->width);
            height_log2 = highest_set_bit_index(mip_texture->height);
            draw_control_count = mip_texture->draw_control_count;
            texture_block = mip_texture->texture_block_offset;
            gs_block_base = gs_texture_allocation_base >> 8;
            palette_block = mip_texture->palette_block_offset + gs_block_base;
            mip_block_0 = mip_texture->mip_block_offset_0 + gs_block_base;
            mip_block_1 = mip_texture->mip_block_offset_1 + gs_block_base;
            mip_block_2 = mip_texture->mip_block_offset_2 + gs_block_base;
            mip_packet->tex1.d = (s64)(((u64)(draw_control_count - 1) << 2) | ((u64)1 << 5) | ((u64)material_shift << 6)) | ((s64)material_base << 32);
            mip_packet->tex0.d = (s64)((u64)((s64)palette_block | ((s64)mip_width_units[0] << 14) | ((s64)0x13 << 20) | ((s64)width_log2 << 26)) | ((u64)height_log2 << 30) | ((u64)1 << 34) | ((u64)(texture_block + gs_block_base) << 37)) | ((s64)4 << 61);
            draw_word = (u64)mip_block_0 | ((u64)mip_width_units[1] << 14);
            draw_word |= (u64)mip_block_1 << 20;
            draw_word |= (u64)mip_width_units[2] << 34;
            draw_word |= (u64)mip_block_2 << 40;
            draw_word |= (u64)mip_width_units[3] << 54;
            mip_packet->miptbp1 = draw_word;
        } else {
            s32 template_index;
            s32 template_slot;
            u64 template_tex0;
            u64 template_tex1;
            u64 template_mip;
            u64 draw_bits;
            u64 template_draw_word;

            for (template_index = 0; resident_material_templates[(template_index + 1) * 3] != 0;
                 template_index++) {
            }
            template_tex0 = resident_material_templates[template_index * 3];
            template_slot = template_index * 3;
            template_tex1 = resident_material_templates[template_slot + 1];
            template_mip = resident_material_templates[template_slot + 2];
            draw_bits = ((u64)material_shift << 6) | 0x20;
            template_draw_word = (template_tex1 & 0x1C) | draw_bits | ((u64)material_base << 32);
            mip_packet->tex0.d = template_tex0;
            mip_packet->miptbp1 = template_mip;
            mip_packet->tex1.d = template_draw_word;
            /* Fakematch: a second set of mip_texture keeps cse from reusing the width
               load, so the bit index call reads mip_texture->width again as retail does. */
            mip_texture = 0;
        }
    }
    slot_materials = &shrub_render_class_material_maps[shrub_render_class_slot_by_id[class_id]];
    qcopy(slot_materials, material_map);
    for (group_index = 0; group_index < render_class->group_count; group_index++) {
        group_header = &render_class->groups[group_index].group->group_header;
        packet = (ShrubRenderPacket *)((((u8 *)group_header) +
                                        (group_header->prefix_record_count * 16)) +
                                       0x10);
        for (index = 0; index < group_header->count; index++) {
            s32 material_index;
            s32 material_base;
            s32 draw_shift;
            s32 draw_high;
            s32 material_shift;

            material_index = slot_materials->b[packet->material_index];
            draw_high = packet->draw_high;
            draw_shift = packet->draw_shift;
            material_base = packet->material_base;
            material_shift = packet->material_shift;
            if (textures != 0) {
                ResidentRenderTextureDefinition *texture;
                s32 width_bits;
                s32 width_units_64;
                s32 width_units_128;
                s32 width_log2;
                s32 height_log2;
                s32 gs_block_base;
                s32 mip_block_1;
                s32 mip_block_0;
                s32 draw_control_count;
                u64 tex0_word;
                u64 tex1_word;
                u64 mip_word;
                u64 clamp_word;

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
                width_log2 = highest_set_bit_index(width_bits >> 16);
                height_log2 = highest_set_bit_index(texture->height);
                gs_block_base = gs_texture_allocation_base >> 8;
                draw_control_count = texture->draw_control_count;
                width_bits = texture->clut_block_offset + gs_block_base;
                mip_block_0 = texture->mip_block_offset_0 + gs_block_base;
                mip_block_1 = texture->mip_block_offset_1 + gs_block_base;
                tex1_word = (s64)(((u64)(draw_control_count - 1) << 2) | ((u64)1 << 5) | ((u64)draw_shift << 6)) | ((s64)draw_high << 32);
                clamp_word = (u64)material_base | ((u64)material_shift << 2) | ((u64)material_index << 24);
                mip_word = (s64)(((u64)width_units_128 << 14) | ((u64)mip_block_0 << 20) | ((u64)1 << 34) | ((u64)mip_block_1 << 40)) | ((s64)1 << 54);
                tex0_word = (s64)((u64)(((s64)width_units_64 << 14) | ((s64)0x13 << 20) | ((s64)width_log2 << 26)) | ((u64)height_log2 << 30) | ((u64)1 << 34) | ((u64)width_bits << 37)) | ((s64)4 << 61);
                *(u64 *)&packet->draw_high = tex1_word;
                *(u64 *)&packet->material_base = clamp_word;
                *(u64 *)&packet->material_index = mip_word;
                *(u64 *)((u8 *)packet + 0x30) = tex0_word;
            } else {
                s32 template_index;
                u64 template_tex0;
                u64 template_tex1;
                u64 template_mip;
                u64 draw_bits;
                u64 template_draw_word;
                u64 template_clamp_word;

                template_tex0 = resident_material_templates[material_index * 3];
                template_index = material_index * 3;
                template_tex1 = resident_material_templates[template_index + 1];
                template_mip = resident_material_templates[template_index + 2];
                draw_bits = ((u64)draw_shift << 6) | 0x20;
                template_draw_word = (template_tex1 & 0x1C) | draw_bits | ((u64)draw_high << 32);
                template_clamp_word = (u64)material_base | ((u64)material_shift << 2) |
                                      ((u64)material_index << 24);
                *(u64 *)&packet->draw_high = template_draw_word;
                *(u64 *)&packet->material_base = template_clamp_word;
                *(u64 *)&packet->material_index = template_mip;
                *(u64 *)((u8 *)packet + 0x30) = template_tex0;
            }
            packet++;
        }
    }
}

extern __typeof__(register_shrub_render_class) func_00203B08 __attribute__((alias("FUN_00203b08")));

s16 shrub_render_class_ids[64] = {0};
