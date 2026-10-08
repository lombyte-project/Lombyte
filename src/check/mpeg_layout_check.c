/* Built only by `make layout-check-sys` (never linked). gcc 2.95 has no
   _Static_assert: a negative array size fails the compile. */
#define OFFSET_CHECK(name, type, field, off) \
    typedef char offset_check_##name[ \
        ((unsigned long)&((type *)0)->field == (off)) ? 1 : -1]
#define SIZE_CHECK(name, type, size) \
    typedef char size_check_##name[(sizeof(type) == (size)) ? 1 : -1]

#include "rnc/sdk/libmpeg.h"

OFFSET_CHECK(sce_frame_count, struct sceMpeg, frameCount, 0x8);
OFFSET_CHECK(sce_sys, struct sceMpeg, sys, 0x40);

OFFSET_CHECK(ref_width, struct MpegRefImage, width, 0x4);
OFFSET_CHECK(ref_status, struct MpegRefImage, status, 0x28);
OFFSET_CHECK(ref_unk60, struct MpegRefImage, unk60, 0x60);
OFFSET_CHECK(mb_ipu_out, struct MpegMbBuffer, ipu_out, 0x4);
OFFSET_CHECK(mb_chroma, struct MpegMbBuffer, chroma, 0xB8);
OFFSET_CHECK(mb_unk128, struct MpegMbBuffer, unk128, 0x128);
OFFSET_CHECK(mb_fetch_count, struct MpegMbBuffer, fetch_count, 0x12C);
OFFSET_CHECK(mb_unk13C, struct MpegMbBuffer, unk13C, 0x13C);
SIZE_CHECK(mb_buffer, struct MpegMbBuffer, 0x140);

OFFSET_CHECK(ipu_dma_state, struct MpegDecoder, ipu_dma_state, 0x4C);
OFFSET_CHECK(unk80, struct MpegDecoder, unk80, 0x80);
OFFSET_CHECK(unk88, struct MpegDecoder, unk88, 0x88);
OFFSET_CHECK(frame_base, struct MpegDecoder, frame_base, 0xAC);
OFFSET_CHECK(unkD8, struct MpegDecoder, unkD8, 0xD8);
OFFSET_CHECK(out_max_width, struct MpegDecoder, out_max_width, 0xDC);
OFFSET_CHECK(out_max_size, struct MpegDecoder, out_max_size, 0xE4);
OFFSET_CHECK(unkF8, struct MpegDecoder, unkF8, 0xF8);
OFFSET_CHECK(frame_buffers, struct MpegDecoder, frame_buffers, 0xFC);
OFFSET_CHECK(frame_count, struct MpegDecoder, frame_count, 0x118);
OFFSET_CHECK(second_field, struct MpegDecoder, second_field, 0x120);
OFFSET_CHECK(horizontal_size, struct MpegDecoder, horizontal_size, 0x124);
OFFSET_CHECK(vbv_buffer_size, struct MpegDecoder, vbv_buffer_size, 0x138);
OFFSET_CHECK(matrix_coefficients, struct MpegDecoder, matrix_coefficients, 0x144);
OFFSET_CHECK(display_vertical_size, struct MpegDecoder, display_vertical_size, 0x14C);
OFFSET_CHECK(picture_coding_type, struct MpegDecoder, picture_coding_type, 0x150);
OFFSET_CHECK(f_code_forward_h, struct MpegDecoder, f_code_forward_h, 0x164);
OFFSET_CHECK(picture_structure, struct MpegDecoder, picture_structure, 0x174);
OFFSET_CHECK(progressive_frame, struct MpegDecoder, progressive_frame, 0x188);
OFFSET_CHECK(unk1AC, struct MpegDecoder, unk1AC, 0x1AC);
OFFSET_CHECK(quantiser_scale_code, struct MpegDecoder, quantiser_scale_code, 0x1B4);
OFFSET_CHECK(ref_images, struct MpegDecoder, ref_images, 0x1B8);
OFFSET_CHECK(mb_buf, struct MpegDecoder, mb_buf, 0x590);
OFFSET_CHECK(mb_buf_index, struct MpegDecoder, mb_buf_index, 0x810);
OFFSET_CHECK(mc_work, struct MpegDecoder, mc_work, 0x81C);
OFFSET_CHECK(load_intra_quantiser_matrix, struct MpegDecoder, load_intra_quantiser_matrix, 0x840);
OFFSET_CHECK(unk848, struct MpegDecoder, unk848, 0x848);
OFFSET_CHECK(mpeg, struct MpegDecoder, mpeg, 0x858);
SIZE_CHECK(cb_data, struct sceMpegCbData, 0x20);
