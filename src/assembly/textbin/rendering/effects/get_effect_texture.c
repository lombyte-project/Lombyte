#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/rendering/effects/get_effect_texture/FUN_001f44b8.s", FUN_001f44b8);
#else

#include "types.h"

struct EffectTextureDefinition {
    s64 tex0;
    u16 texel_offset_quadwords;
    u16 palette_offset_quadwords;
    s16 width_log2;
    s16 height_log2;
};

struct TextureUploadPacket {
    s32 palette_address;
    s16 reserved_zero;
    s16 palette_block_offset;
    s32 texel_address;
    u8 width_log2;
    u8 height_log2;
    s16 texel_block_offset;
};

extern s32 gs_texture_allocation_cursor __asm__("D_0015EE74");
extern s32 pending_texture_upload_count __asm__("D_0015F458");
extern s32 level_texture_payload_address __asm__("D_0015F460");
extern struct TextureUploadPacket pending_texture_uploads[] __asm__("D_0018D040");
extern struct EffectTextureDefinition effect_texture_definitions[] __asm__("D_0018D440");

s64 get_effect_texture(s32 index) __asm__("FUN_001f44b8");

s64 get_effect_texture(s32 index) {
    struct EffectTextureDefinition *texture;
    struct TextureUploadPacket *upload;
    s32 width_log2;
    s32 buffer_width_shift;
    s32 palette_block_offset;
    s32 texel_address;
    s32 texel_block_offset;
    s64 tex0_word;
    s64 width_bits;
    s64 palette_bits;
    s32 pending_texture_upload_count_snapshot;

    texture = &effect_texture_definitions[index];
    if (texture->tex0 == 0) {
        width_log2 = texture->width_log2;
        buffer_width_shift = width_log2 - 6;
        palette_block_offset = gs_texture_allocation_cursor >> 8;
        texel_address = gs_texture_allocation_cursor + 0x400;
        texel_block_offset = texel_address >> 8;
        pending_texture_upload_count_snapshot = pending_texture_upload_count;
        tex0_word = texel_block_offset;
        tex0_word |= ((u64)(1 << ((buffer_width_shift <= -1) ? 0 : buffer_width_shift)) << 14);
        /* Retail sign-extends each 16-bit exponent before packing TEX0. */
        width_bits = ((s64)((u64)(u16)texture->width_log2 << 48) >> 22);
        width_bits |= 0x1300000;
        tex0_word |= width_bits;
        tex0_word |= (s64)((u64)(u16)texture->height_log2 << 48) >> 18;
        palette_bits = (u64)palette_block_offset << 37;
        palette_bits |= (u64)1 << 34;
        tex0_word |= palette_bits;
        tex0_word |= (u64)1 << 63;
        gs_texture_allocation_cursor = texel_address + (1 << (width_log2 + texture->height_log2));
        texture->tex0 = tex0_word;
        if (pending_texture_upload_count_snapshot < 0x40) {
            upload = &pending_texture_uploads[pending_texture_upload_count_snapshot];
            upload->palette_address = level_texture_payload_address + texture->palette_offset_quadwords * 0x10;
            upload->palette_block_offset = palette_block_offset;
            upload->reserved_zero = 0;
            *(s32 *)((u8 *)pending_texture_uploads + pending_texture_upload_count_snapshot * 0x10 + 8) = level_texture_payload_address + texture->texel_offset_quadwords * 0x10;
            upload->width_log2 = (u8)texture->width_log2;
            upload->height_log2 = (u8)texture->height_log2;
            upload->texel_block_offset = texel_block_offset;
            pending_texture_upload_count = pending_texture_upload_count_snapshot + 1;
        }
    }
    return effect_texture_definitions[index].tex0;
}

extern __typeof__(get_effect_texture) func_001F44B8 __attribute__((alias("FUN_001f44b8")));

#endif /* NON_MATCHING */
