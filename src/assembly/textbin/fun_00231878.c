#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00231878/FUN_00231878.s", FUN_00231878);
#else
#include "types.h"
#include "eetypes.h"

typedef struct { u128 data[6]; } sceGsLoadImage;

struct CommonArchiveMemory {
    u8 pad_0[0x14];
    s32 archive_base;
};

struct LoadingSlideDiscEntry {
    u8 pad_0[0x1388];
    s32 start_sector;
    s32 sector_count;
};

extern u8 D_00137B80[];
extern s32 D_0015EE74;
extern s32 D_0015EE78;
extern s32 D_0015EE8C;
extern struct CommonArchiveMemory D_001940C0;
extern void FlushCache(s32 a0);
extern s32 sceCdSync(s32 a0);
extern s32 sceGsExecLoadImage(sceGsLoadImage *load_image, s32 image_address);
extern s32 sceGsSetDefLoadImage(sceGsLoadImage *load_image, s32 x, s32 y, s32 w, s32 h, s32 a4, s32 a6, s32 a7);
extern s32 wait_for_graphics_pipeline_idle(s32 a0, s32 a1) __asm__("func_00120558");
extern s32 func_0020B618(s32 a0, s32 a1);
extern s32 submit_audio_stream_io_request(s32 a0, s32 a1, s32 a2, s32 a3) __asm__("func_00216728");

void prepare_loading_slide_textures(s32 language_index, s32 first_slide, s32 second_slide, s64 *shared_texture, s32 *first_texture, s32 *second_texture) __asm__("FUN_00231878");

void prepare_loading_slide_textures(s32 language_index, s32 first_slide, s32 second_slide, s64 *shared_texture, s32 *first_texture, s32 *second_texture) {
    s32 texture_bases[6];
    sceGsLoadImage load_image;
    s32 *first_output;
    s32 *second_output;
    s32 i;
    s32 *texture_base_output;
    s32 upload_bytes;
    s32 image_address;
    s32 archive_base;
    s32 *first_slide_offsets;
    s32 *second_slide_offsets;
    u8 *disc_entry;
    struct CommonArchiveMemory *archive_memory;

    first_output = (s32 *)first_texture;
    second_output = (s32 *)second_texture;
    archive_memory = &D_001940C0;
    disc_entry = D_00137B80 + language_index * 8;
    i = 0;
    submit_audio_stream_io_request(archive_memory->archive_base + 0x100000, ((struct LoadingSlideDiscEntry *)disc_entry)->start_sector,
        ((struct LoadingSlideDiscEntry *)disc_entry)->sector_count, archive_memory->archive_base);
    sceCdSync(0);
    FlushCache(0);
    func_0020B618(archive_memory->archive_base + 0x100000, archive_memory->archive_base);
    FlushCache(0);
    archive_base = archive_memory->archive_base;
    first_slide_offsets = (s32 *)((u8 *)archive_base + first_slide * 4);
    second_slide_offsets = (s32 *)((u8 *)archive_base + second_slide * 4);
    D_0015EE78 = D_0015EE8C;
    D_0015EE74 = D_0015EE8C;
    texture_base_output = texture_bases;
    for (i = 0; i < 6; i++) {
        if (i == 0) {
            sceGsSetDefLoadImage(&load_image, (D_0015EE74 << 8) >> 16, 1, 0, 0, 0, 0x10, 0x10);
            upload_bytes = 0x400;
            image_address = (s32)((u8 *)archive_base + *(s32 *)((u8 *)archive_base + 4)) + 0x20;
        } else if (i == 1) {
            sceGsSetDefLoadImage(&load_image, (D_0015EE74 << 8) >> 16, 1, 0x13, 0, 0, 0x40, 0x40);
            upload_bytes = 0x1000;
            image_address = (s32)((u8 *)archive_base + *(s32 *)((u8 *)archive_base + 4)) + 0x420;
        } else if (i == 2) {
            sceGsSetDefLoadImage(&load_image, (D_0015EE74 << 8) >> 16, 1, 0, 0, 0, 0x10, 0x10);
            upload_bytes = 0x400;
            image_address = (s32)((u8 *)archive_base + first_slide_offsets[2]) + 0x20;
        } else if (i == 3) {
            sceGsSetDefLoadImage(&load_image, (D_0015EE74 << 8) >> 16, 8, 0x13, 0, 0, 0x200, 0x40);
            upload_bytes = 0x8000;
            image_address = (s32)((u8 *)archive_base + first_slide_offsets[2]) + 0x420;
        } else if (i == 4) {
            sceGsSetDefLoadImage(&load_image, (D_0015EE74 << 8) >> 16, 1, 0, 0, 0, 0x10, 0x10);
            upload_bytes = 0x400;
            image_address = (s32)((u8 *)archive_base + second_slide_offsets[2]) + 0x20;
        } else {
            sceGsSetDefLoadImage(&load_image, (D_0015EE74 << 8) >> 16, 8, 0x13, 0, 0, 0x200, 0x40);
            upload_bytes = 0x8000;
            image_address = (s32)((u8 *)archive_base + second_slide_offsets[2]) + 0x420;
        }
        FlushCache(0);
        sceGsExecLoadImage(&load_image, image_address);
        wait_for_graphics_pipeline_idle(0, 0);
        *texture_base_output = D_0015EE74 >> 8;
        D_0015EE74 = D_0015EE74 + upload_bytes;
        texture_base_output++;
    }
    *shared_texture = (texture_bases[1] | 0x19304000) | (((s64)texture_bases[0] << 37) | ((s64)0xB000 << 19)) | (((s64)-1) << 63);
    *(s64 *)first_output = (texture_bases[3] | 0x25320000) | (((s64)texture_bases[2] << 37) | ((s64)0xB000 << 19)) | (((s64)-1) << 63);
    *(s64 *)second_output = (texture_bases[5] | 0x25320000) | (((s64)texture_bases[4] << 37) | ((s64)0xB000 << 19)) | (((s64)-1) << 63);
}
#endif /* NON_MATCHING */
