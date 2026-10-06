#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00231878/FUN_00231878.s", FUN_00231878);
#else
#include "types.h"

typedef struct {
    u64 data[12];
} sceGsLoadImage __attribute__((aligned(16)));

struct CommonArchiveMemory {
    u8 pad_0[0x14];
    s32 archive_base;
    u64 texture_bits;
    u64 image_bits;
};

struct LoadingSlideDiscEntry {
    u8 pad_0[0x1388];
    s32 start_sector;
    s32 sector_count;
};

extern u8 D_00137B80[];
extern s32 gs_texture_allocation_cursor __asm__("D_0015EE74");
extern s32 gs_texture_allocation_start __asm__("D_0015EE78");
extern s32 gs_texture_allocation_base __asm__("D_0015EE8C");
extern struct CommonArchiveMemory D_001940C0;
extern void FlushCache(s32 a0);
extern s32 sceCdSync(s32 a0);
extern s32 sceGsExecLoadImage(sceGsLoadImage *load_image, s32 image_address);
extern s32 sceGsSetDefLoadImage(sceGsLoadImage *load_image, s32 block_offset, s32 buffer_width,
                                s32 pixel_storage_format, s32 x, s32 y, s32 width, s32 height);
extern s32 wait_for_graphics_pipeline_idle(s32 a0, s32 a1) __asm__("func_00120558");
extern s32 decompress_wad(s32 source_address, s32 destination_address) __asm__("func_0020B618");
extern s32 submit_cd_read_request(void *a0, u32 a1, u32 a2) __asm__("func_00216728");

void prepare_loading_slide_textures(s32 language_index, s32 first_slide, s32 second_slide,
                                    u64 *shared_texture, u64 *first_texture,
                                    u64 *second_texture) __asm__("FUN_00231878");

void prepare_loading_slide_textures(s32 language_index, s32 first_slide, s32 second_slide,
                                    u64 *shared_texture, u64 *first_texture, u64 *second_texture) {
    s32 texture_bases[6];
    sceGsLoadImage load_image;
    u64 *first_output;
    u64 *second_output;
    s32 upload_index;
    s32 *texture_base_output;
    s32 upload_bytes;
    s32 image_address;
    s32 archive_base;
    u64 texture_bits;
    u64 image_bits;
    s32 disc_start_sector;
    s32 disc_sector_count;
    struct CommonArchiveMemory *archive_memory;

    first_output = first_texture;
    second_output = second_texture;
    archive_memory = &D_001940C0;
    upload_index = 0;
    disc_start_sector =
        ((struct LoadingSlideDiscEntry *)(D_00137B80 + language_index * 8))->start_sector;
    disc_sector_count = *(s32 *)(D_00137B80 + 0x138c + language_index * 8);
    submit_cd_read_request((void *)(archive_memory->archive_base + 0x100000), disc_start_sector,
                           disc_sector_count);
    sceCdSync(0);
    FlushCache(0);
    decompress_wad(archive_memory->archive_base + 0x100000, archive_memory->archive_base);
    FlushCache(0);
    archive_base = archive_memory->archive_base;
    gs_texture_allocation_start = gs_texture_allocation_base;
    gs_texture_allocation_cursor = gs_texture_allocation_base;
    texture_base_output = texture_bases;
    for (upload_index = 0; upload_index < 6; upload_index++) {
        if (upload_index == 0) {
            sceGsSetDefLoadImage(&load_image, (gs_texture_allocation_cursor << 8) >> 16, 1, 0, 0, 0,
                                 0x10, 0x10);
            upload_bytes = 0x400;
            image_address = (s32)((u8 *)archive_base + *(s32 *)((u8 *)archive_base + 4)) + 0x20;
        } else if (upload_index == 1) {
            sceGsSetDefLoadImage(&load_image, (gs_texture_allocation_cursor << 8) >> 16, 1, 0x13, 0,
                                 0, 0x40, 0x40);
            upload_bytes = 0x1000;
            image_address = (s32)((u8 *)archive_base + *(s32 *)((u8 *)archive_base + 4)) + 0x420;
        } else if (upload_index == 2) {
            sceGsSetDefLoadImage(&load_image, (gs_texture_allocation_cursor << 8) >> 16, 1, 0, 0, 0,
                                 0x10, 0x10);
            upload_bytes = 0x400;
            image_address =
                (s32)((u8 *)archive_base + *(s32 *)((u8 *)archive_base + first_slide * 4 + 8)) +
                0x20;
        } else if (upload_index == 3) {
            sceGsSetDefLoadImage(&load_image, (gs_texture_allocation_cursor << 8) >> 16, 8, 0x13, 0,
                                 0, 0x200, 0x40);
            upload_bytes = 0x8000;
            image_address =
                (s32)((u8 *)archive_base + *(s32 *)((u8 *)archive_base + first_slide * 4 + 8)) +
                0x420;
        } else if (upload_index == 4) {
            sceGsSetDefLoadImage(&load_image, (gs_texture_allocation_cursor << 8) >> 16, 1, 0, 0, 0,
                                 0x10, 0x10);
            upload_bytes = 0x400;
            image_address =
                (s32)((u8 *)archive_base + *(s32 *)((u8 *)archive_base + second_slide * 4 + 8)) +
                0x20;
        } else {
            sceGsSetDefLoadImage(&load_image, (gs_texture_allocation_cursor << 8) >> 16, 8, 0x13, 0,
                                 0, 0x200, 0x40);
            upload_bytes = 0x8000;
            image_address =
                (s32)((u8 *)archive_base + *(s32 *)((u8 *)archive_base + second_slide * 4 + 8)) +
                0x420;
        }
        FlushCache(0);
        sceGsExecLoadImage(&load_image, image_address);
        wait_for_graphics_pipeline_idle(0, 0);
        *texture_base_output = gs_texture_allocation_cursor >> 8;
        gs_texture_allocation_cursor = gs_texture_allocation_cursor + upload_bytes;
        texture_base_output++;
    }
    /* Each TEX0 combines an image base with its paired palette base. */
    texture_bits = ((u64)texture_bases[0] << 37) | (0xB000ULL << 19);
    image_bits = (u64)(texture_bases[1] | 0x19304000);
    *shared_texture = (image_bits | texture_bits) | (1ULL << 63);
    *first_output = (texture_bases[3] | 0x25320000) |
                    (((u64)texture_bases[2] << 37) | (0xB000ULL << 19)) | (1ULL << 63);
    *second_output = (((u64)texture_bases[4] << 37) | (0xB000ULL << 19)) |
                     (texture_bases[5] | 0x25320000) | (1ULL << 63);
}

extern __typeof__(prepare_loading_slide_textures) func_00231878
    __attribute__((alias("FUN_00231878")));
#endif /* NON_MATCHING */
