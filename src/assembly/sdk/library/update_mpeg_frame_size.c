#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/update_mpeg_frame_size/func_0012C4C8.s",
            func_0012C4C8);
#else

#include "rnc/sdk/libmpeg.h"

extern s32 InitializeReferenceImage();
extern s32 _initRefImages();
extern s32 copy_object_field_backward() __asm__("func_0012BC10");
extern s32 reserve_aligned_buffer_space() __asm__("func_0012BC20");

void func_0012C4C8(struct sceMpeg *mpeg) {
    u8 *field_image0;
    u8 *field_image1;
    u8 *field_image2;
    u8 *field_image3;
    u8 *field_image4;
    u8 *field_image5;
    s32 field_height;
    s32 height;
    s32 width;
    s32 mpeg2;
    s32 mb_height;
    u32 frame_size;
    s32 aligned_height;
    s32 aligned_width;
    s32 half_height;
    u8 *buffer_pool;
    u8 *frame_image0;
    u8 *frame_image1;
    u8 *frame_image2;
    struct MpegDecoder *decoder;

    decoder = mpeg->sys;
    mpeg2 = decoder->unk848;
    if (mpeg2 == 0) {
        decoder->picture_structure = 3;
        decoder->unk13C = 1;
        decoder->unk140 = 1;
        decoder->progressive_frame = 1;
        decoder->frame_pred_frame_dct = 1;
        decoder->matrix_coefficients = 5;
    }
    decoder->mb_width = (s32)((s32)(decoder->horizontal_size + 0xF) >> 4);
    if (mpeg2 != 0) {
        if (decoder->unk13C == 0) {
            mb_height = ((s32)(decoder->vertical_size + 0x1F) >> 5) * 2;
        } else {
            goto block_6;
        }
    } else {
    block_6:
        mb_height = (s32)(decoder->vertical_size + 0xF) >> 4;
    }
    decoder->mb_height = mb_height;
    aligned_height = mb_height << 4;
    aligned_width = decoder->mb_width << 4;
    if (aligned_width == mpeg->width) {
        if (aligned_height == mpeg->height) {
            return;
        }
        field_image5 = ((u8 *)decoder + 0x528);
    } else {
        field_image5 = ((u8 *)decoder + 0x528);
    }
    buffer_pool = field_image5 - 0x420;
    {
        mpeg->height = aligned_height;
        mpeg->width = aligned_width;
        field_image0 = ((u8 *)decoder + (0x320));
        frame_image0 = ((u8 *)decoder + (0x1E8));
        field_image1 = ((u8 *)decoder + (0x388));
        frame_image1 = ((u8 *)decoder + (0x250));
        frame_image2 = ((u8 *)decoder + (0x2B8));
        field_image2 = ((u8 *)decoder + (0x3F0));
        frame_size = ((0x180U * (u32)aligned_width) * (u32)aligned_height) >> 8;
        field_image3 = ((u8 *)decoder + (0x458));
        field_image4 = ((u8 *)decoder + (0x4C0));
        width = aligned_width;
        height = aligned_height;
        copy_object_field_backward(buffer_pool);
        decoder->frame_buffers[0] =
            reserve_aligned_buffer_space(decoder, buffer_pool, frame_size, 0x40);
        decoder->frame_buffers[1] =
            reserve_aligned_buffer_space(decoder, buffer_pool, frame_size, 0x40);
        decoder->frame_buffers[2] =
            reserve_aligned_buffer_space(decoder, buffer_pool, frame_size, 0x40);
        _initRefImages(frame_image0, frame_image1, frame_image2, field_image0, field_image1,
                       field_image2, field_image3, field_image4, field_image5,
                       decoder->frame_buffers[0], decoder->frame_buffers[1],
                       decoder->frame_buffers[2], width, height);
        InitializeReferenceImage(frame_image0, width, height);
        InitializeReferenceImage(frame_image1, width, height);
        InitializeReferenceImage(frame_image2, width, height);
        half_height = height >> 1;
        field_height = half_height;
        InitializeReferenceImage(field_image0, width, field_height);
        InitializeReferenceImage(field_image1, width, field_height);
        InitializeReferenceImage(field_image2, width, field_height);
        InitializeReferenceImage(field_image3, width, field_height);
        InitializeReferenceImage(field_image4, width, field_height);
        InitializeReferenceImage(field_image5, width, field_height);
    }
}
#endif /* NON_MATCHING */
