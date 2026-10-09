#ifndef LOMBYTE_RNC_RENDERING_TEXTURE_UPLOAD_H
#define LOMBYTE_RNC_RENDERING_TEXTURE_UPLOAD_H

#include "types.h"

/* Pending CLUT + image transfer to GS memory (D_0018D040, 64 entries,
   count in D_0015F458). Queued by FUN_00204e30, fun_00204cf0 and
   get_frame_texture; the block fields are the TEX0 values they return. */
struct TextureUpload {
    s32 clut_data;  /* 0x00: EE address of the CLUT (fun_00204cf0: texture CLUT pointer) */
    s16 unk4;       /* 0x04: every writer stores 0 */
    s16 cbp;        /* 0x06: GS block of the CLUT, TEX0.CBP */
    s32 image_data; /* 0x08: EE address of the pixels (fun_00204cf0: texture pixel pointer) */
    u8 tw;          /* 0x0C: log2 width, TEX0.TW */
    u8 th;          /* 0x0D: log2 height, TEX0.TH */
    s16 tbp;        /* 0x0E: GS block of the pixels, TEX0.TBP0 */
}; /* size 0x10 */

#define TEXTURE_UPLOAD_MAX 0x40

extern struct TextureUpload pending_texture_uploads[0x40] __asm__("D_0018D040");

/* 8-bit image file as streamed or embedded (fun_00204cf0 reads it; the
   save screen preview in fun_0021f990 and the map textures in
   draw_map_screen use the same CLUT +0x20 / pixels +0x420 split). */
struct ClutImage {
    u8 pad0[0x8];
    s32 width;  /* 0x008: pixels; FUN_001f97a0 turns it into TEX0.TW */
    s32 height; /* 0x00C: pixels; TEX0.TH */
    u8 pad10[0x10];
    u8 clut[0x400]; /* 0x020: 256 RGBA entries */
    u8 pixels[1];   /* 0x420: width * height indices */
};

#endif /* LOMBYTE_RNC_RENDERING_TEXTURE_UPLOAD_H */
