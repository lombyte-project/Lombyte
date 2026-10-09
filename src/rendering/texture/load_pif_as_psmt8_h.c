#include "types.h"

/* EE layouts: PIF header 0x20 bytes, scratch descriptor 0x54 bytes,
   sceGsLoadImage packet 0x60 bytes (16-byte aligned), output 0x18 bytes.
   Only the fields accessed by retail are named; unused header/descriptor
   bytes remain opaque rather than assigning them guessed meanings. */
typedef struct {
    u64 q[12];
} GsLoadImage __attribute__((aligned(16)));

typedef struct {
    u8 *palette;                 /* 0x00: header + 0x20 */
    u8 *image;                   /* 0x04: palette + palette_size */
    u8 pad08[0xC];
    s32 palette_size;            /* 0x14: bytes */
    s32 image_size;              /* 0x18: width * height bytes */
    u8 pad1C[0x10];
    s32 texel_block_offset;      /* 0x2C: GS byte destination >> 8 */
    u8 pad30[0xC];
    s32 buffer_width;            /* 0x3C: 64-pixel units, at least one */
    u8 pad40[0xC];
    s32 width_log2;              /* 0x4C: highest-set-bit index */
    s32 height_log2;             /* 0x50: highest-set-bit index */
} PifTex;

typedef struct {
    u8 pad00[8];
    s32 width;                   /* 0x08 */
    s32 height;                  /* 0x0C */
    u8 pad10[4];
    s32 palette_storage_format;  /* 0x14: GS CPSM, not a byte count */
    u8 pad18[8];
} PifHeader;

typedef struct {
    u64 tex0;
    u64 tex1;
    u64 clamp;
} GsTextureRegisters;

extern void FillTransferWords(void *, int, int);
extern s32 highest_set_bit_index(s32) __asm__("func_001F97A0");
extern s32 sceGsSetDefLoadImage(GsLoadImage *, short, short, short, short, short, short, short);
extern void FlushCache(int);
extern s32 sceGsExecLoadImage(GsLoadImage *, void *);
extern s32 wait_for_graphics_pipeline_idle(int, unsigned short) __asm__("func_00120558");

/* Retail 0x001e9168..0x001e9337, also called by load_debug_font:
   palette = header + 0x20; format zero selects 0x400 palette bytes,
   every nonzero format selects 0x200. No format/header/bounds validation is
   performed. Format zero is PSMCT32; a nonzero value is passed through,
   so this routine alone does not establish which 16-bit formats occur.
   Image = palette + palette_size; PSMT8H (0x1B) consumes one byte per texel.

   Upload the palette as 16x16, DBW=1, then the image as width x height,
   DBW=max(width >> 6, 1). Each upload has FlushCache(0), execution and an
   idle wait in that order. All seven upload parameters narrow to signed
   16 bits; retail re-loads the low halfwords of dimensions and format.
   Destinations are signed 32-bit GS byte addresses shifted right by eight;
   no alignment rounding/check is added. The packet is 16-byte aligned.
   sceGsSetDefLoadImage puts BITBLTBUF/TRXPOS/TRXREG/TRXDIR in q[2..9]
   and the image GIFtag in q[10..11]. For PSMT8H, QWC is the signed
   narrowed width * height shifted right by four, not rounded up; the
   execution helper reads its low 15 bits. The stored image_size alone
   does not determine the actual DMA length. Output stores require 8-byte
   alignment. Payloads must satisfy the GS DMA quadword transfer contract;
   neither this routine nor its execution helper repairs misalignment.

   TEX0 = TBP0 | TBW<<14 | PSMT8H<<20 | TW<<26 | TH<<30 | TCC<<34 |
          CBP<<37 | CPSM<<51 | CLD<<61, with TCC=1 and CLD=4.
   Other fields remain zero for valid inputs. Retail does not mask the
   input fields, so preserve sign extension and spill into adjacent fields.
   The output is three consecutive 64-bit words: TEX0, TEX1=1, CLAMP=0.
   Dimensions need not be powers of two for this code to run: TW/TH use
   highest_set_bit_index, not a rounded logarithm. No upload return is tested.
 */
void load_pif_as_psmt8_h(PifHeader *pif, GsTextureRegisters *registers,
                         s32 texel_destination, s32 palette_destination)
    __asm__("FUN_001e9168");

void load_pif_as_psmt8_h(PifHeader *pif, GsTextureRegisters *registers,
                         s32 texel_destination, s32 palette_destination) {
    PifTex upload;
    GsLoadImage load_image;
    s32 palette_block_offset;
    u64 tex0_word;
    u64 format_word;
    u64 size_word;

    FillTransferWords(&upload, 0, sizeof(upload));
    upload.palette = (u8 *)pif + 0x20;
    if (pif->palette_storage_format == 0) {
        upload.palette_size = 0x400;
    } else {
        upload.palette_size = 0x200;
    }
    palette_block_offset = palette_destination >> 8;
    upload.width_log2 = highest_set_bit_index(pif->width);
    upload.height_log2 = highest_set_bit_index(pif->height);
    upload.image = (u8 *)pif + (upload.palette_size + 0x20);
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
    format_word = (u64)(s64)0x1B << 20;
    tex0_word = (u64)(s64)upload.texel_block_offset | ((u64)(s64)upload.buffer_width << 14);
    size_word = ((u64)(s64)upload.width_log2 << 26) | format_word;
    tex0_word |= size_word;
    tex0_word |= (u64)(s64)upload.height_log2 << 30;
    {
        /* Its own temporary: reusing format_word changes retail's registers. */
        u64 palette_word = (u64)(s64)palette_block_offset << 37;

        palette_word |= (u64)(s64)1 << 34;
        tex0_word |= palette_word;
    }
    tex0_word |= (u64)(s64)pif->palette_storage_format << 51;
    tex0_word |= (u64)1 << 63;
    registers->tex0 = tex0_word;
    registers->tex1 = 1;
    registers->clamp = 0;
}

extern __typeof__(load_pif_as_psmt8_h) func_001E9168 __attribute__((alias("FUN_001e9168")));

