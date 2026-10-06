#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/textbin/rendering/texture/load_pif_as_psmt8_h/FUN_001e9168.s",
    FUN_001e9168);
#else

#include "types.h"

/* sceGsLoadImage */
typedef struct {
    u64 q[12];
} GsLoadImage __attribute__((aligned(16)));

typedef struct {
    void *palette; /* 0x00 */
    void *image;   /* 0x04 */
    int unk08[3];
    int palette_size; /* 0x14 */
    int image_size;   /* 0x18 */
    int unk1C[4];
    int texel_block_offset; /* 0x2C */
    int unk30[3];
    int buffer_width; /* 0x3C */
    int unk40[3];
    int width_log2;  /* 0x4C */
    int height_log2; /* 0x50 */
} PifTex;            /* 0x54 */

typedef struct {
    char unk00[8];
    int width;  /* 0x08 */
    int height; /* 0x0C */
    int unk10;
    int palette_storage_format; /* 0x14 */
    u8 pad18[8];
} PifHeader;

extern void FillTransferWords(void *, int, int);
extern s32 highest_set_bit_index(s32) __asm__("func_001F97A0");
extern s32 sceGsSetDefLoadImage(GsLoadImage *, short, short, short, short, short, short, short);
extern void FlushCache(int);
extern s32 sceGsExecLoadImage(GsLoadImage *, void *);
extern s32 wait_for_graphics_pipeline_idle(int, unsigned short) __asm__("func_00120558");

/* LoadPifAsPSMT8H: uploads a PIF image (header image, CLUT at +0x20 of
   0x200 bytes when +0x14 is set, else 0x400, then width * height pixels) to GS
   memory: the 16x16 CLUT to block palette_destination >> 8, the pixels as PSMT8H to
   texel_destination >> 8 with a buffer width of width / 64 (at least 1), and writes the
   matching TEX0 (SCE_GS_SET_TEX0 with TCC 1, CLD 4) plus 1, 0 to registers.
   The header's fields are ints cast to short at the call, which is where
   retail's second, narrower load of each comes from. The descriptor is a
   zeroed stack struct, so its fields are re-read after the calls. */
void load_pif_as_psmt8_h(void *image, void *registers, int texel_destination,
                         int palette_destination) __asm__("FUN_001e9168");

void load_pif_as_psmt8_h(void *image, void *registers, int texel_destination,
                         int palette_destination) {
    PifTex upload;
    GsLoadImage load_image;
    PifHeader *pif = image;
    u64 *out = registers;
    int palette_block_offset;
    s64 tex0_word;
    s64 field_word;

    FillTransferWords(&upload, 0, sizeof(upload));
    upload.palette = (char *)pif + 0x20;
    if (pif->palette_storage_format == 0) {
        upload.palette_size = 0x400;
    } else {
        upload.palette_size = 0x200;
    }
    palette_block_offset = palette_destination >> 8;
    upload.width_log2 = highest_set_bit_index(pif->width);
    upload.height_log2 = highest_set_bit_index(pif->height);
    upload.image = (char *)pif + (upload.palette_size + 0x20);
    upload.image_size = pif->width * pif->height;
    sceGsSetDefLoadImage(&load_image, palette_block_offset, 1, (short)pif->palette_storage_format,
                         0, 0, 16, 16);
    FlushCache(0);
    sceGsExecLoadImage(&load_image, upload.palette);
    wait_for_graphics_pipeline_idle(0, 0);
    upload.buffer_width = pif->width >> 6;
    if (upload.buffer_width <= 0) {
        upload.buffer_width = 1;
    }
    upload.texel_block_offset = texel_destination >> 8;
    sceGsSetDefLoadImage(&load_image, upload.texel_block_offset, upload.buffer_width, 0x1B, 0, 0,
                         (short)pif->width, (short)pif->height);
    FlushCache(0);
    sceGsExecLoadImage(&load_image, upload.image);
    wait_for_graphics_pipeline_idle(0, 0);
    field_word = (s64)0x1B << 20;
    tex0_word = (s64)upload.texel_block_offset;
    tex0_word |= (s64)upload.buffer_width << 14;
    tex0_word |= field_word | ((s64)upload.width_log2 << 26);
    tex0_word |= (s64)upload.height_log2 << 30;
    field_word = ((s64)palette_block_offset << 37) | ((s64)1 << 34);
    tex0_word |= field_word;
    tex0_word |= (s64)pif->palette_storage_format << 51;
    tex0_word |= (u64)1 << 63;
    out[0] = tex0_word;
    out[1] = 1;
    out[2] = 0;
}

extern __typeof__(load_pif_as_psmt8_h) func_001E9168 __attribute__((alias("FUN_001e9168")));

#endif /* NON_MATCHING */
