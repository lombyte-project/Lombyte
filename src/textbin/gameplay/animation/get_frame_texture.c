#include "types.h"
#include "asm.h"

#include "types.h"
#include "rnc/globals.h"
#include "rnc/rendering/texture_upload.h"

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

extern s32 gs_texture_allocation_base __asm__("D_0015EE8C");
extern s32 pending_texture_upload_count __asm__("D_0015F458");
struct FrameTextureTables {
    u8 pad00[0x20];
    s32 frame_references_address;
    s32 image_pages_address;
    s32 palette_pages_address;
};
extern struct FrameTextureTables frame_texture_tables __asm__("D_0019A3E8");

u64 get_frame_texture(s32 frame_id) __asm__("FUN_001ffa10");

u64 get_frame_texture(s32 frame_id) {
    struct FrameTextureRef *frame;
    struct FramePalettePage *palette_page;
    struct FrameImagePage *image_page;
    struct TextureUpload *packet;
    struct TextureUpload *initial_packet;
    struct TextureUpload *palette_packet;
    u32 packet_offset;
    u32 queued_transfer;
    s32 image_upload_count;
    s32 allocation_cursor;
    u8 width_log2;
    u8 height_log2;
    s32 palette_source_address;
    s32 return_mode;
    s32 return_shift;
    u64 tex0_word;
    u64 palette_word;

    frame =
        (struct FrameTextureRef *)(frame_id * 4 + frame_texture_tables.frame_references_address);
    palette_page = (struct FramePalettePage *)(frame_texture_tables.palette_pages_address +
                                               frame->palette_index * 8);
    image_page = (struct FrameImagePage *)(frame_texture_tables.image_pages_address +
                                           frame->image_index * 8);
    queued_transfer = 0;

    if (palette_page->gs_block_offset == 0 || image_page->gs_block_offset == 0) {
        /* Retail fills this first packet even when the queue is already full. */
        packet_offset = pending_texture_upload_count * 0x10;
        palette_source_address = palette_page->source_address;
        initial_packet = pending_texture_uploads + pending_texture_upload_count;
        initial_packet->clut_data = palette_source_address;
        initial_packet->unk4 = 0;
        initial_packet->cbp = 0x3FF0;
        *(s32 *)((u8 *)pending_texture_uploads + packet_offset + 8) = palette_page->source_address;
        initial_packet->tw = 5;
        initial_packet->th = 5;
        initial_packet->tbp = 0x3FF0;
    }

    if (palette_page->gs_block_offset == 0) {
        palette_page->gs_block_offset = gs_texture_allocation_cursor >> 8;
        gs_texture_allocation_cursor += 0x400;
        if (pending_texture_upload_count < 0x40) {
            queued_transfer = 1;
            palette_packet = pending_texture_uploads + pending_texture_upload_count;
            palette_packet->clut_data = palette_page->source_address;
            palette_packet->unk4 = 0;
            palette_packet->cbp = palette_page->gs_block_offset;
        }
    }

    if (image_page->gs_block_offset == 0) {
        height_log2 = image_page->height_log2;
        width_log2 = image_page->width_log2;
        allocation_cursor = gs_texture_allocation_cursor;
        image_page->gs_block_offset = allocation_cursor >> 8;
        if (height_log2 < width_log2) {
            height_log2 += width_log2 - height_log2;
        }
        gs_texture_allocation_cursor = allocation_cursor + (1 << (height_log2 * 2));
        image_upload_count = pending_texture_upload_count;
        if (image_upload_count < 0x40) {
            queued_transfer = 1;
            packet = pending_texture_uploads + image_upload_count;
            *(s32 *)((u8 *)pending_texture_uploads + image_upload_count * 0x10 + 8) =
                image_page->source_address;
            packet->tw = image_page->width_log2;
            packet->th = image_page->height_log2;
            packet->tbp = image_page->gs_block_offset;
        }
    }

    if (queued_transfer != 0) {
        pending_texture_upload_count += 1;
    }

    return_shift = image_page->width_log2 - 6;
    if (return_shift < 0) {
        return_shift = 0;
    }
    return_mode = 0x13;
    if (image_page->gs_block_offset < (gs_texture_allocation_base >> 8)) {
        return_mode = 0x1B;
    }
    return_shift = 1 << return_shift;
    tex0_word = image_page->gs_block_offset | ((u64)return_shift << 14);
    tex0_word |= (u64)return_mode << 20;
    tex0_word |= (u64)image_page->width_log2 << 26;
    tex0_word |= (u64)image_page->height_log2 << 30;
    palette_word = (u64)palette_page->gs_block_offset << 37;
    palette_word |= (u64)0x8000 << 19;
    tex0_word |= palette_word;
    tex0_word |= (u64)-1 << 63;
    return tex0_word;
}

extern __typeof__(get_frame_texture) func_001FFA10 __attribute__((alias("FUN_001ffa10")));
