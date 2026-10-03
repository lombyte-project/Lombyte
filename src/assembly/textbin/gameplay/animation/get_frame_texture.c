#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/gameplay/animation/get_frame_texture/FUN_001ffa10.s", FUN_001ffa10);
#else
#include "types.h"

struct FrameTextureRef {
    s16 palette_index;
    s16 image_index;
};

struct FramePalettePage {
    s32 source_address;
    u16 gs_block_offset;
};

struct FrameImagePage {
    s32 source_address;
    u16 gs_block_offset;
    u8 width_log2;
    u8 height_log2;
};

struct GifTexturePacket {
    s32 palette_address;
    u16 reserved_zero;
    u16 palette_block_offset;
    s32 image_address;
    u8 image_width;
    u8 image_height;
    u16 image_base;
};

extern s32 gs_texture_allocation_cursor __asm__("D_0015EE74");
extern s32 gs_texture_allocation_base __asm__("D_0015EE8C");
extern s32 pending_texture_upload_count __asm__("D_0015F458");
extern u8 pending_texture_uploads[] __asm__("D_0018D040");
struct FrameTextureTables {
    u8 pad00[0x20];
    s32 frame_references_address;
    s32 image_pages_address;
    s32 palette_pages_address;
};
extern struct FrameTextureTables frame_texture_tables __asm__("D_0019A3E8");

s64 get_frame_texture(s32 frame_id) __asm__("FUN_001ffa10");

s64 get_frame_texture(s32 frame_id) {
    struct FrameTextureRef *frame;
    struct FramePalettePage *palette_page;
    struct FrameImagePage *image_page;
    struct GifTexturePacket *packet;
    s32 queued_transfer;
    u32 width_log2;
    u32 height_log2;
    s32 packet_offset;
    s32 allocation_start;
    s32 return_mode;
    s32 return_shift;
    s64 tex0_word;
    s64 palette_word;

    frame = (struct FrameTextureRef *)(frame_id * 4 + frame_texture_tables.frame_references_address);
    palette_page = (struct FramePalettePage *)(frame_texture_tables.palette_pages_address + frame->palette_index * 8);
    image_page = (struct FrameImagePage *)(frame_texture_tables.image_pages_address + frame->image_index * 8);
    queued_transfer = 0;

    if (palette_page->gs_block_offset == 0 || image_page->gs_block_offset == 0) {
        packet_offset = pending_texture_upload_count * 0x10;
        packet = (struct GifTexturePacket *)(pending_texture_uploads + packet_offset);
        packet->palette_address = palette_page->source_address;
        packet->reserved_zero = 0;
        packet->palette_block_offset = 0x3FF0;
        *(s32 *)((u8 *)(pending_texture_uploads + packet_offset) + 8) = palette_page->source_address;
        packet->image_width = 5;
        packet->image_height = 5;
        packet->image_base = 0x3FF0;
    }

    if (palette_page->gs_block_offset == 0) {
        palette_page->gs_block_offset = gs_texture_allocation_cursor >> 8;
        gs_texture_allocation_cursor += 0x400;
        if (pending_texture_upload_count < 0x40) {
            packet = (struct GifTexturePacket *)(pending_texture_uploads + pending_texture_upload_count * 0x10);
            queued_transfer = 1;
            packet->palette_address = palette_page->source_address;
            packet->reserved_zero = 0;
            packet->palette_block_offset = palette_page->gs_block_offset;
        }
    }

    if (image_page->gs_block_offset == 0) {
        allocation_start = gs_texture_allocation_cursor;
        height_log2 = image_page->height_log2;
        width_log2 = image_page->width_log2;
        image_page->gs_block_offset = allocation_start >> 8;
        if (width_log2 < height_log2) {
            width_log2 = height_log2;
        }
        /* Retail masks the exponent before the word-sized allocation shift. */
        gs_texture_allocation_cursor = allocation_start + (1 << ((width_log2 & 0xF) * 2));
        if (pending_texture_upload_count < 0x40) {
            packet = (struct GifTexturePacket *)(pending_texture_uploads + pending_texture_upload_count * 0x10);
            queued_transfer = 1;
            packet->image_address = image_page->source_address;
            packet->image_width = image_page->width_log2;
            packet->image_height = image_page->height_log2;
            packet->image_base = image_page->gs_block_offset;
        }
    }

    if (queued_transfer != 0) {
        pending_texture_upload_count += 1;
    }

    return_shift = image_page->width_log2 - 6;
    if (return_shift < 0) {
        return_shift = 0;
    }
    return_mode = image_page->gs_block_offset < (gs_texture_allocation_base >> 8) ? 0x1B : 0x13;
    tex0_word = image_page->gs_block_offset | ((s64)(1 << return_shift) << 14);
    tex0_word |= (s64)return_mode << 20;
    tex0_word |= (s64)image_page->width_log2 << 26;
    tex0_word |= (s64)image_page->height_log2 << 30;
    palette_word = (s64)palette_page->gs_block_offset << 37;
    palette_word |= (s64)0x8000 << 19;
    tex0_word |= palette_word;
    tex0_word |= (s64)-1 << 63;
    return tex0_word;
}

extern __typeof__(get_frame_texture) func_001FFA10 __attribute__((alias("FUN_001ffa10")));

#endif /* NON_MATCHING */
