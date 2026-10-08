#ifndef LOMBYTE_RNC_SDK_LIBMPEG_H
#define LOMBYTE_RNC_SDK_LIBMPEG_H

#include "types.h"

/* libmpeg (SDK).  struct sceMpeg follows the public SDK header; its sys
 * pointer leads to the library's private decoder state (struct MpegDecoder).
 * Bitstream fields follow the MPEG-2 syntax order of the parsers that fill
 * them (_sequenceHeader, _pictureCodingExtension, _sequenceDisplayExtension,
 * _quantMatrixExtension).  Offset checks: src/check/mpeg_layout_check.c. */

struct MpegDecoder;

struct sceMpeg {
    s32 width;
    s32 height;
    s32 frameCount;
    u8 pad_C[0x4];
    s64 pts;
    s64 dts;
    u64 flags;
    s64 pts2nd;
    s64 dts2nd;
    u64 flags2nd;
    struct MpegDecoder *sys;            /* private decoder state */
};

/* A decoded reference picture (ref_images[] below).  _isOutSizeOK formats
 * width/height into "Too small buffer size for %dx%d picture". */
struct MpegRefImage {
    u8 *data;                           /* picture base; _getRef0 adds macroblock index * 0x180 */
    s32 width;
    s32 height;
    s32 unkC;
    s32 unk10;                          /* non-zero: sized check in _isOutSizeOK;
                                           _getRef0 macroblock index x * unk10 + y */
    u8 pad_14[0x14];
    s32 status;                         /* 1: output by _dispRefImage; ClearMpegReferenceBuffer zeroes it */
    u8 pad_2C[0x18];
    s32 unk44;
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
    s32 unk60;
};

typedef void (*MpegMcFunc)(void);

/* One queued reference fetch of a macroblock part (_getRef0). */
struct MpegMcFetch {
    u8 *addr;
    s32 x;
    s32 n0;
    s32 n1;
    s32 step;
    u8 *p0;
    u8 *p1;
};

/* One of two macroblock work areas, swapped through mb_buf_index.
 * _clearOnce points them at the scratchpad (0x70000000 / 0x70001B00).
 * _getRef0 queues up to four luma/chroma reference fetches per macroblock
 * (count in fetch_count) for _doMC. */
struct MpegMbBuffer {
    u32 spr_base;                       /* SPR 0x70000000 or 0x70001B00; fetch n at + n * 0x600 */
    u32 ipu_out;                        /* spr_base + 0x1800; 0x300 bytes received from the IPU (_decMB0) */
    u8 *luma_ref[4];                    /* reference picture luma source per fetch */
    u8 *chroma_ref[4];                  /* reference picture chroma source per fetch */
    MpegMcFunc luma_func[4];            /* D_00132E30[] copy routine per fetch */
    MpegMcFunc chroma_func[4];          /* D_00132E50[] copy routine per fetch */
    struct MpegMcFetch luma[4];
    struct MpegMcFetch chroma[4];
    s32 unk128;                         /* read by _doMC */
    s32 fetch_count;                    /* queued fetches; loop count in _doMC */
    s32 unk130;                         /* read by _doMC */
    u8 pad_134[0x4];
    s32 unk138;                         /* _doMC skips the work when 0 */
    s32 unk13C;                         /* set 1 by _decMB0/_skipMB0, 0 by _slice0 */
};

struct MpegDecoder {
    s32 unk0;
    s32 unk4;                           /* non-zero: _outputFrame displays a frame */
    s32 unk8;
    u8 pad_C[0x40];
    u8 ipu_dma_state;                   /* &ipu_dma_state passed to suspend_mpeg_decoder_ipu_dma */
    u8 pad_4D[0x33];
    s32 unk80;
    u8 pad_84[0x4];
    u64 unk88;
    u8 pad_90[0x4];
    s32 unk94;
    s32 unk98;
    s32 unk9C;
    s32 unkA0;
    s32 unkA4;
    s32 unkA8;
    s32 frame_base;                     /* sceMpeg.frameCount = frame_count - frame_base (_sceMpegFlush) */
    s32 unkB0;                          /* non-zero: _dispRefImage uses _csc_storeRefImage, else _cpr8 */
    s32 unkB4;
    s32 unkB8;
    s32 unkBC;
    s32 unkC0;
    s32 unkC4;
    s32 unkC8;
    s32 unkCC;
    s32 unkD0;
    s32 unkD4;                          /* 0 in _sequenceHeader, set by _pictureCodingExtension */
    s32 unkD8;                          /* output address (_cpr8 masks it to physical) */

    /* Output size limits, checked by _isOutSizeOK against the picture. */
    s32 out_max_width;                  /* >= picture width when out_max_height != 0 */
    s32 out_max_height;                 /* >= picture height; 0: check out_max_size */
    s32 out_max_size;                   /* >= unkC * unk10 of the picture */

    u8 pad_E8[0x10];
    s32 unkF8;                          /* 1 -> 2 in _outputFrame */
    s32 frame_buffers[3];               /* reserved in _sequenceHeader, passed to _initRefImages */
    u8 pad_108[0x10];
    s32 frame_count;                    /* +1 per decoded frame (_decodeOrSkipFrame) */
    s32 unk11C;
    s32 second_field;                   /* toggled per field picture (_decodeOrSkipFrame); set at
                                           _lastFrame: "the second field is missing" */

    /* sequence_header (_sequenceHeader) */
    u32 horizontal_size;                /* 12 bits */
    s32 vertical_size;                  /* 12 bits; > 2800 is an error */
    s32 mb_width;                       /* (horizontal_size + 15) >> 4 */
    s32 mb_height;
    u32 bit_rate;                       /* 18 bits */
    s32 vbv_buffer_size;                /* 10 bits */
    s32 unk13C;                         /* 0: mb_height rounded to 32-line pairs (_sequenceHeader) */
    s32 unk140;

    /* sequence_display_extension */
    s32 matrix_coefficients;            /* 3rd 8-bit colour_description field */
    s32 display_horizontal_size;        /* 14 bits */
    s32 display_vertical_size;          /* 14 bits */

    s32 picture_coding_type;            /* 1 I, 2 P (_skipMB0 zeroes motion), 3 B (_outputFrame) */
    s32 unk154;
    s32 unk158;
    s32 unk15C;
    s32 unk160;

    /* picture_coding_extension */
    s32 f_code_forward_h;
    s32 f_code_forward_v;
    s32 f_code_backward_h;
    s32 f_code_backward_v;
    s32 picture_structure;              /* 3: frame picture */
    s32 top_field_first;
    s32 frame_pred_frame_dct;
    s32 concealment_motion_vectors;
    s32 repeat_first_field;
    s32 progressive_frame;

    u8 pad_18C[0x20];
    s32 unk1AC;
    s32 dc_reset;                       /* DCR bit of the IPU BDEC command (_decMB0); set by _skipMB0 */
    s32 quantiser_scale_code;           /* 5 bits from the slice header (_sliceB); QSC of BDEC */
    struct MpegRefImage *ref_images[12]; /* chosen by picture_structure / picture_coding_type in _outputFrame */
    u8 pad_1E8[0x3A8];
    struct MpegMbBuffer mb_buf[2];
    s32 mb_buf_index;                   /* toggled per slice in _slice0 */
    u8 pad_814[0x8];
    u8 *mc_work;                        /* _getRef0: destination of the queued fetches */
    s32 unk820;
    u8 pad_824[0x1C];

    /* quant_matrix_extension / sequence_header */
    s32 load_intra_quantiser_matrix;    /* set -> IPU SETIQ intra (0x50000000) */
    s32 load_non_intra_quantiser_matrix;/* set -> IPU SETIQ non-intra (0x58000000) */
    s32 unk848;                         /* 0: _sequenceHeader loads frame-picture defaults */
    s32 unk84C;
    s32 unk850;
    s32 unk854;
    struct sceMpeg *mpeg;               /* owning sceMpeg (_dispRefImage writes its pts/dts/flags) */
};

/* Callback record passed to sceMpeg callbacks; type selects the event
 * (_setDefaultQM sends 2 before and 3 after its IPU upload). The stream
 * record (read_mpeg's video_callback) has data at +0x08, a signed byte
 * count at +0x0C and signed 64-bit PTS/DTS at +0x10/+0x18. */
struct sceMpegCbData {
    s32 type;
    u8 pad_4[0x1C];
};

typedef s32 (*MpegStreamCallback)(struct sceMpeg *, struct sceMpegCbData *, void *);

extern s32 sceMpegAddStrCallback(struct sceMpeg *, s32, s32, MpegStreamCallback, void *);

#endif
