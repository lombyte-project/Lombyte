#ifndef LOMBYTE_RNC_SDK_LIBMPEG_H
#define LOMBYTE_RNC_SDK_LIBMPEG_H

#include "types.h"

/* libmpeg (SDK).  struct sceMpeg follows the public SDK header layout; its
 * sys pointer leads to the library's private decoder state (struct MpegDecoder).
 * Decoder fields read from the bitstream follow the MPEG-2 syntax order of the
 * extension parsers (_pictureCodingExtension, _sequenceDisplayExtension,
 * _quantMatrixExtension). */

struct MpegDisplayState;
struct MpegDecoder;

struct sceMpeg {
    s32 width;                          /* 0x0 */
    s32 height;                         /* 0x4 */
    s32 frameCount;                     /* 0x8 */
    u8 pad_C[0x4];
    s64 pts;                            /* 0x10 */
    s64 dts;                            /* 0x18 */
    u64 flags;                          /* 0x20 */
    s64 pts2nd;                         /* 0x28 */
    s64 dts2nd;                         /* 0x30 */
    u64 flags2nd;                       /* 0x38 */
    struct MpegDecoder *sys;            /* 0x40 */
};

#define SCEMPEG_OFFSET_CHECK(field, off) \
    typedef char scempeg_offset_check_##field[ \
        ((unsigned long)&((struct sceMpeg *)0)->field == (off)) ? 1 : -1]
SCEMPEG_OFFSET_CHECK(frameCount, 0x8);
SCEMPEG_OFFSET_CHECK(sys, 0x40);

struct MpegDecoder {
    s32 unk0;                           /* 0x0 */
    s32 unk4;                           /* 0x4 */
    s32 unk8;                           /* 0x8 */
    u8 pad_C[0x40];
    u8 ipu_dma_state;                   /* 0x4C: &ipu_dma_state passed to FUN_0012cc20 (suspend_mpeg_decoder_ipu_dma) */
    u8 pad_4D[0x33];
    s32 unk80;                          /* 0x80 */
    u8 pad_84[0x4];
    u64 unk88;                          /* 0x88 */
    u8 pad_90[0x4];
    s32 unk94;                          /* 0x94 */
    s32 unk98;                          /* 0x98 */
    s32 unk9C;                          /* 0x9C */
    s32 unkA0;                          /* 0xA0 */
    s32 unkA4;                          /* 0xA4 */
    s32 unkA8;                          /* 0xA8 */
    s32 frame_base;                     /* 0xAC: sceMpeg.frameCount = frame_count - frame_base (_sceMpegFlush) */
    s32 unkB0;                          /* 0xB0 */
    s32 unkB4;                          /* 0xB4 */
    s32 unkB8;                          /* 0xB8 */
    s32 unkBC;                          /* 0xBC */
    s32 unkC0;                          /* 0xC0 */
    s32 unkC4;                          /* 0xC4 */
    s32 unkC8;                          /* 0xC8 */
    s32 unkCC;                          /* 0xCC */
    s32 unkD0;                          /* 0xD0 */
    s32 unkD4;                          /* 0xD4 */
    s32 unkD8;                          /* 0xD8 */
    s32 unkDC;                          /* 0xDC */
    s32 unkE0;                          /* 0xE0 */
    s32 unkE4;                          /* 0xE4 */
    u8 pad_E8[0x10];
    s32 unkF8;                          /* 0xF8 */
    u8 pad_FC[0x1C];
    s32 frame_count;                    /* 0x118: +1 per decoded frame in _decodeOrSkipFrame; minus frame_base gives sceMpeg.frameCount */
    s32 unk11C;                         /* 0x11C */
    s32 unk120;                         /* 0x120 */
    u32 unk124;                         /* 0x124 */
    s32 unk128;                         /* 0x128 */
    u8 pad_12C[0x8];
    u32 unk134;                         /* 0x134 */
    s32 unk138;                         /* 0x138 */
    u8 pad_13C[0x8];
    s32 matrix_coefficients;            /* 0x144: 3rd 8-bit colour_description field, _sequenceDisplayExtension */
    s32 display_horizontal_size;        /* 0x148: 14-bit field after colour_description, _sequenceDisplayExtension */
    s32 display_vertical_size;          /* 0x14C: 14-bit field after marker bit, _sequenceDisplayExtension */
    s32 unk150;                         /* 0x150 */
    s32 unk154;                         /* 0x154 */
    s32 unk158;                         /* 0x158 */
    s32 unk15C;                         /* 0x15C */
    s32 unk160;                         /* 0x160 */
    s32 f_code_forward_h;               /* 0x164: 1st 4-bit f_code, _pictureCodingExtension */
    s32 f_code_forward_v;               /* 0x168: 2nd 4-bit f_code, _pictureCodingExtension */
    s32 f_code_backward_h;              /* 0x16C: 3rd 4-bit f_code, _pictureCodingExtension */
    s32 f_code_backward_v;              /* 0x170: 4th 4-bit f_code, _pictureCodingExtension */
    s32 picture_structure;              /* 0x174: 2-bit field after intra_dc_precision; ==3 (frame) tested in _decodeOrSkipFrame */
    s32 top_field_first;                /* 0x178: 1-bit, _pictureCodingExtension */
    s32 frame_pred_frame_dct;           /* 0x17C: 1-bit, _pictureCodingExtension */
    s32 concealment_motion_vectors;     /* 0x180: 1-bit, _pictureCodingExtension */
    s32 repeat_first_field;             /* 0x184: 1-bit after the q_scale/vlc/scan IPU_CTRL bits, _pictureCodingExtension */
    s32 progressive_frame;              /* 0x188: 1-bit after chroma_420_type, _pictureCodingExtension */
    u8 pad_18C[0x20];
    s32 unk1AC;                         /* 0x1AC */
    s32 unk1B0;                         /* 0x1B0 */
    s32 unk1B4;                         /* 0x1B4 */
    s32 unk1B8;                         /* 0x1B8 */
    s32 unk1BC;                         /* 0x1BC */
    u8 pad_1C0[0x4];
    s32 unk1C4;                         /* 0x1C4 */
    s32 unk1C8;                         /* 0x1C8 */
    s32 unk1CC;                         /* 0x1CC */
    s32 unk1D0;                         /* 0x1D0 */
    s32 unk1D4;                         /* 0x1D4 */
    s32 unk1D8;                         /* 0x1D8 */
    s32 unk1DC;                         /* 0x1DC */
    u8 pad_1E0[0x4];
    s32 unk1E4;                         /* 0x1E4 */
    u8 pad_1E8[0x3A8];
    s32 unk590;                         /* 0x590 */
    s32 unk594;                         /* 0x594 */
    u8 pad_598[0x138];
    s32 unk6D0;                         /* 0x6D0 */
    s32 unk6D4;                         /* 0x6D4 */
    u8 pad_6D8[0x138];
    s32 unk810;                         /* 0x810 */
    u8 pad_814[0x8];
    s32 unk81C;                         /* 0x81C */
    s32 unk820;                         /* 0x820 */
    u8 pad_824[0x1C];
    s32 load_intra_quantiser_matrix;    /* 0x840: 1-bit; set -> IPU cmd 0x50000000 (SETIQ intra), _quantMatrixExtension */
    s32 load_non_intra_quantiser_matrix;/* 0x844: 1-bit; set -> IPU cmd 0x58000000 (SETIQ non-intra), _quantMatrixExtension */
    s32 unk848;                         /* 0x848 */
    s32 unk84C;                         /* 0x84C */
    s32 unk850;                         /* 0x850 */
    s32 unk854;                         /* 0x854 */
    struct MpegDisplayState *unk858;    /* 0x858 */
};

#define MPEG_OFFSET_CHECK(field, off) \
    typedef char mpeg_offset_check_##field[ \
        ((unsigned long)&((struct MpegDecoder *)0)->field == (off)) ? 1 : -1]
MPEG_OFFSET_CHECK(ipu_dma_state, 0x4C);
MPEG_OFFSET_CHECK(frame_base, 0xAC);
MPEG_OFFSET_CHECK(frame_count, 0x118);
MPEG_OFFSET_CHECK(matrix_coefficients, 0x144);
MPEG_OFFSET_CHECK(display_horizontal_size, 0x148);
MPEG_OFFSET_CHECK(display_vertical_size, 0x14C);
MPEG_OFFSET_CHECK(f_code_forward_h, 0x164);
MPEG_OFFSET_CHECK(f_code_forward_v, 0x168);
MPEG_OFFSET_CHECK(f_code_backward_h, 0x16C);
MPEG_OFFSET_CHECK(f_code_backward_v, 0x170);
MPEG_OFFSET_CHECK(picture_structure, 0x174);
MPEG_OFFSET_CHECK(top_field_first, 0x178);
MPEG_OFFSET_CHECK(frame_pred_frame_dct, 0x17C);
MPEG_OFFSET_CHECK(concealment_motion_vectors, 0x180);
MPEG_OFFSET_CHECK(repeat_first_field, 0x184);
MPEG_OFFSET_CHECK(progressive_frame, 0x188);
MPEG_OFFSET_CHECK(load_intra_quantiser_matrix, 0x840);
MPEG_OFFSET_CHECK(load_non_intra_quantiser_matrix, 0x844);

#endif
