#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00202270/FUN_00202270.s", FUN_00202270);
#else

#include "types.h"
#include "eetypes.h"
#include "sda.h"

typedef struct { u128 data[6]; } sceGsLoadImage;

typedef struct {
    u8 pad0[8];
    s32 width;
    s32 height;
    s32 pixel_storage_format;
    s32 palette_storage_format;
    s32 pad18;
    s32 mip_level_count;
    u8 data[4];
} MipTextureHeader;

typedef struct {
    s32 palette_address;
    s32 mip_addresses[4];
    s32 palette_size;
    s32 mip_sizes[4];
    s32 palette_block_offset;
    s32 texture_block_offsets[4];
    s32 buffer_widths[4];
    s32 width_log2;
    s32 height_log2;
} MipTextureUpload;

extern s32 gs_texture_allocation_cursor __asm__("D_0015EE74") MACRO_ADDR;
extern void FillTransferWords(void *dst, s32 value, s32 size) __asm__("func_001F97E8");
extern void FlushCache(s32);
extern s32 sceGsSetDefLoadImage(sceGsLoadImage *, s16, s16, s16, s16, s16, s16, s16);
extern s32 sceGsExecLoadImage(sceGsLoadImage *, u128 *);
extern s32 wait_for_graphics_pipeline_idle(s32, s32) __asm__("func_00120558");
extern s32 highest_set_bit_index(s32) __asm__("func_001F97A0");

s32 upload_mip_texture(MipTextureHeader *tex, u64 *regs) __asm__("FUN_00202270");

s32 upload_mip_texture(MipTextureHeader *tex, u64 *regs) {
    MipTextureUpload upload;
    sceGsLoadImage load_image;
    s32 mip_index;
    s32 allocation_bytes;
    s32 *buffer_width;
    u64 tex0_word;
    u64 palette_word;
    u64 mip_word;

    FillTransferWords(&upload, 0, sizeof(upload));
    buffer_width = upload.buffer_widths;
    switch (tex->pixel_storage_format) {
    default:
        break;
    case 0:
    case 2:
        upload.palette_address = 0;
        upload.palette_size = 0;
        break;
    case 0x13:
    case 0x14:
        upload.palette_address = (s32)tex->data;
        if (tex->pixel_storage_format == 0x14) {
            if (tex->palette_storage_format == 0) {
                upload.palette_size = 0x40;
            } else {
                upload.palette_size = 0x20;
            }
        } else {
            if (tex->palette_storage_format != 0) {
                upload.palette_size = 0x200;
            } else {
                upload.palette_size = 0x400;
            }
        }
        break;
    }
    upload.width_log2 = highest_set_bit_index(tex->width);
    upload.height_log2 = highest_set_bit_index(tex->height);
    upload.mip_addresses[0] = (s32)tex->data + upload.palette_size;
    switch (tex->pixel_storage_format) {
    case 0:
        upload.mip_sizes[0] = tex->width * tex->height * 4;
        break;
    case 2:
        upload.mip_sizes[0] = tex->width * tex->height * 2;
        break;
    case 0x13:
        upload.mip_sizes[0] = tex->width * tex->height;
        break;
    case 0x14:
        upload.mip_sizes[0] = (tex->width * tex->height) >> 1;
        break;
    }
    if (tex->pixel_storage_format == 0x13 || tex->pixel_storage_format == 0x14) {
        upload.palette_block_offset = gs_texture_allocation_cursor >> 8;
        if (tex->pixel_storage_format == 0x14) {
            gs_texture_allocation_cursor += 0x100;
            sceGsSetDefLoadImage(&load_image, upload.palette_block_offset, 1, tex->palette_storage_format, 0, 0, 8, 2);
        } else {
            gs_texture_allocation_cursor += upload.palette_size;
            sceGsSetDefLoadImage(&load_image, upload.palette_block_offset, 1, tex->palette_storage_format, 0, 0, 16, 16);
        }
        FlushCache(0);
        sceGsExecLoadImage(&load_image, (u128 *)upload.palette_address);
        wait_for_graphics_pipeline_idle(0, 0);
    }
    for (mip_index = 1; mip_index < tex->mip_level_count; mip_index++) {
        upload.mip_sizes[mip_index] = upload.mip_sizes[mip_index - 1] >> 2;
        upload.mip_addresses[mip_index] = upload.mip_addresses[mip_index - 1] + upload.mip_sizes[mip_index - 1];
    }
    for (mip_index = 0; mip_index < tex->mip_level_count; mip_index++) {
        *buffer_width = tex->width >> (mip_index + 6);
        if (*buffer_width <= 0) {
            *buffer_width = 1;
        }
        upload.texture_block_offsets[mip_index] = gs_texture_allocation_cursor >> 8;
        sceGsSetDefLoadImage(&load_image, upload.texture_block_offsets[mip_index], *buffer_width, tex->pixel_storage_format, 0, 0, tex->width >> mip_index, tex->height >> mip_index);
        buffer_width++;
        FlushCache(0);
        sceGsExecLoadImage(&load_image, (u128 *)upload.mip_addresses[mip_index]);
        wait_for_graphics_pipeline_idle(0, 0);
        allocation_bytes = upload.mip_sizes[0] >> (mip_index * 2);
        if (allocation_bytes <= 0xFF) {
            allocation_bytes = 0x100;
        }
        gs_texture_allocation_cursor += allocation_bytes;
    }
    tex0_word = upload.texture_block_offsets[0];
    tex0_word |= (u64)upload.buffer_widths[0] << 14;
    tex0_word |= (u64)tex->pixel_storage_format << 20;
    tex0_word |= (u64)upload.width_log2 << 26;
    tex0_word |= (u64)upload.height_log2 << 30;
    palette_word = (u64)upload.palette_block_offset << 37;
    palette_word |= (u64)1 << 34;
    tex0_word |= palette_word;
    tex0_word |= (u64)tex->palette_storage_format << 51;
    tex0_word |= (u64)1 << 63;
    regs[0] = tex0_word;
    mip_word = upload.texture_block_offsets[1];
    mip_word |= (u64)upload.buffer_widths[1] << 14;
    mip_word |= (u64)upload.texture_block_offsets[2] << 20;
    mip_word |= (u64)upload.buffer_widths[2] << 34;
    mip_word |= (u64)upload.texture_block_offsets[3] << 40;
    mip_word |= (u64)upload.buffer_widths[3] << 54;
    regs[1] = ((u64)(tex->mip_level_count - 1) << 2) | 0xFFA0000000E0;
    regs[2] = mip_word;
    return -1;
}

extern __typeof__(upload_mip_texture) func_00202270 __attribute__((alias("FUN_00202270")));

#endif /* NON_MATCHING */
