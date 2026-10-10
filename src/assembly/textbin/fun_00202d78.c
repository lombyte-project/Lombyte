#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00202d78/FUN_00202d78.s", FUN_00202d78);
#else
#include "types.h"
#include "rnc/rendering/resident_class.h"

extern s32 gs_texture_allocation_base __asm__("D_0015EE8C");
#include "rnc/rendering/material_templates.h"
extern s32 highest_set_bit_index(s32) __asm__("func_001F97A0");

void build_indexed_resident_render_packet(u64 *packet,
                                          struct ResidentRenderTextureDefinition *texture,
                                          s32 draw_high, s32 draw_shift, s32 material_base,
                                          s32 material_shift,
                                          s32 material_index) __asm__("FUN_00202d78");

void build_indexed_resident_render_packet(u64 *packet,
                                          struct ResidentRenderTextureDefinition *texture,
                                          s32 draw_high, s32 draw_shift, s32 material_base,
                                          s32 material_shift, s32 material_index) {
    s32 width_units_128;
    s32 width_units_64;
    s32 width_log2;
    s64 height_log2;
    s32 gs_block_base;
    s32 texture_block;
    s32 mip_block_1;
    s32 mip_block_0;
    u64 *fallback_packet;
    u64 draw_control_word;
    u64 texture_word;
    u64 texture_address_word;
    u64 mip_word;
    u64 mip_address_word;
    s32 draw_control_count;

    width_units_64 = texture->width >> 6;
    width_units_128 = texture->width >> 7;
    if (width_units_64 <= 0) {
        width_units_64 = 1;
    }
    if (width_units_128 <= 0) {
        width_units_128 = 1;
    }
    width_log2 = highest_set_bit_index(texture->width);
    height_log2 = highest_set_bit_index(texture->height);
    gs_block_base = gs_texture_allocation_base >> 8;
    mip_block_1 = texture->mip_block_offset_1 + gs_block_base;
    texture_block = texture->clut_block_offset + gs_block_base;
    mip_block_0 = texture->mip_block_offset_0 + gs_block_base;
    /* Preserve sparse writes: every other 64-bit packet word is untouched. */
    draw_control_count = texture->draw_control_count;
    if (material_index >= 0) {
        draw_control_word = (u64)draw_shift;
        draw_control_word <<= 6;
        draw_control_word |= 0x20;
        draw_control_word |= ((u64)(draw_control_count - 1) << 2);
        draw_control_word |= (u64)draw_high << 32;
        packet[0] = draw_control_word;
        packet += 2;
        packet[0] = material_base | ((u64)material_shift << 2) | ((u64)material_index << 24);
        packet += 2;
        texture_word = ((u64)width_log2 << 26) | 0x1300000;
        texture_word = ((u64)width_units_64 << 14) | texture_word;
        texture_word |= (u64)height_log2 << 30;
        texture_address_word = ((u64)0x8000 << 19) | ((u64)texture_block << 37);
        texture_word |= texture_address_word;
        texture_word |= (u64)-1 << 63;
        mip_word = ((u64)width_units_128 << 14) | ((u64)mip_block_0 << 20);
        mip_address_word = ((u64)mip_block_1 << 40) | ((u64)0x8000 << 19);
        mip_word |= mip_address_word;
        mip_word |= (u64)0x8000 << 39;
        packet[2] = mip_word;
        packet[0] = texture_word;
    } else if (material_index < -1) {
        fallback_packet = special_material_template;
        if (material_index == -3) {
            fallback_packet = alternate_special_material_template;
        }
        packet[0] = ((u64)draw_shift << 6) | 0x20 | ((u64)draw_high << 32);
        packet += 2;
        packet[0] = 5;
        packet += 2;
        packet[0] = fallback_packet[0];
        packet[2] = fallback_packet[2];
    } else {
        draw_control_word = ((u64)draw_shift << 6) | 0x20 | ((u64)draw_high << 32);
        packet[0] = draw_control_word;
        packet += 2;
        packet[0] = 5;
        packet += 2;
        packet[0] = ((((u64)0x8000 << 29) | 0x9980) << 19) | 0x7FFB;
        packet[2] = 0;
    }
}

extern __typeof__(build_indexed_resident_render_packet) func_00202D78
    __attribute__((alias("FUN_00202d78")));

#endif /* NON_MATCHING */
