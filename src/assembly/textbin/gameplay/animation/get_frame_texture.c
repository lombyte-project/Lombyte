#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/gameplay/animation/get_frame_texture/FUN_001ffa10.s", FUN_001ffa10);
#else
#include "types.h"
#include "rnc/gameplay_animation_get_frame_texture_types.h"

struct FrameTextureRef {
    s16 texture;
    s16 image;
};

struct FrameTexturePage {
    s32 address;
    u16 base;
};

struct FrameImagePage {
    s32 address;
    u16 base;
    u8 width;
    u8 height;
};

struct GifTexturePacket {
    s32 texture_address;
    u16 texture_base;
    u16 texture_width;
    s32 image_address;
    u8 image_width;
    u8 image_height;
    u16 image_base;
};

extern s32 D_0015EE74;
extern s32 D_0015EE8C;
extern s32 D_0015F458;
extern u8 D_0018D040[];
extern struct M2c_D_0019A3E8 D_0019A3E8;

s64 get_frame_texture(s32 frame_id) __asm__("FUN_001ffa10");

s64 get_frame_texture(s32 frame_id) {
    struct FrameTextureRef *frame;
    struct FrameTexturePage *texture_page;
    struct FrameImagePage *image_page;
    struct GifTexturePacket *packet;
    s32 frame_allocated;
    u32 width;
    u32 height;
    s32 packet_offset;
    s32 allocation_start;
    s32 return_width;
    s32 return_base;
    s32 return_mode;
    s64 return_mask;
    s32 return_shift;
    s64 ret;
    s64 tex0;
    s64 return_top;

    frame = (struct FrameTextureRef *)(frame_id * 4 + D_0019A3E8.unk20);
    texture_page = (struct FrameTexturePage *)(D_0019A3E8.unk28 + frame->texture * 8);
    image_page = (struct FrameImagePage *)(D_0019A3E8.unk24 + frame->image * 8);
    frame_allocated = 0;

    if (texture_page->base == 0 || image_page->base == 0) {
        packet_offset = D_0015F458 * 0x10;
        packet = (struct GifTexturePacket *)(D_0018D040 + packet_offset);
        packet->texture_address = texture_page->address;
        packet->texture_base = 0;
        packet->texture_width = 0x3FF0;
        *(s32 *)((u8 *)(D_0018D040 + packet_offset) + 8) = texture_page->address;
        packet->image_width = 5;
        packet->image_height = 5;
        packet->image_base = 0x3FF0;
    }

    if (texture_page->base == 0) {
        texture_page->base = D_0015EE74 >> 8;
        D_0015EE74 += 0x400;
        if (D_0015F458 < 0x40) {
            packet = (struct GifTexturePacket *)(D_0018D040 + D_0015F458 * 0x10);
            frame_allocated = 1;
            packet->texture_address = texture_page->address;
            packet->texture_base = 0;
            packet->texture_width = texture_page->base;
        }
    }

    if (image_page->base == 0) {
        allocation_start = D_0015EE74;
        height = image_page->height;
        width = image_page->width;
        image_page->base = allocation_start >> 8;
        if (width < height) {
            width = height;
        }
        D_0015EE74 = allocation_start + (1 << (width * 2));
        if (D_0015F458 < 0x40) {
            packet = (struct GifTexturePacket *)(D_0018D040 + D_0015F458 * 0x10);
            frame_allocated = 1;
            packet->image_address = image_page->address;
            packet->image_width = image_page->width;
            packet->image_height = image_page->height;
            packet->image_base = image_page->base;
        }
    }

    if (frame_allocated != 0) {
        D_0015F458 += 1;
    }

    return_shift = image_page->width - 6;
    if (return_shift < 0) {
        return_shift = 0;
    }
    return_mode = image_page->base < (D_0015EE8C >> 8) ? 0x1B : 0x13;
    ret = image_page->base | ((s64)(1 << return_shift) << 14);
    ret |= (s64)return_mode << 20;
    ret |= (s64)image_page->width << 26;
    ret |= (s64)image_page->height << 30;
    tex0 = (s64)texture_page->base << 37;
    tex0 |= (s64)0x8000 << 19;
    ret |= tex0;
    ret |= (s64)-1 << 63;
    return ret;
}
#endif /* NON_MATCHING */
