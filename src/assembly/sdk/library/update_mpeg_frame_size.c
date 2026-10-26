#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/update_mpeg_frame_size/func_0012C4C8.s",
            func_0012C4C8);
#else
/* Near miss (sdk BYTES 22/170, size-exact): left is the schedule of the frame_size spill and the
 * mpeg->height/width stores. */
#include "rnc/sdk/libmpeg.h"

extern s32 InitializeReferenceImage();
extern s32 _initRefImages();
extern s32 copy_object_field_backward() __asm__("func_0012BC10");
extern s32 reserve_aligned_buffer_space() __asm__("func_0012BC20");

void func_0012C4C8(struct sceMpeg *mpeg) {
    struct MpegDecoder *decoder;
    u8 *frame0;
    u8 *frame1;
    u8 *frame2;
    u8 *field0;
    u8 *field1;
    u8 *field2;
    u8 *field3;
    u8 *field4;
    u8 *field5;
    u8 *pool;
    s32 mpeg2;
    s32 mb_height;
    s32 width;
    s32 height;
    u32 frame_size;

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
    decoder->mb_width = (s32)(decoder->horizontal_size + 15) >> 4;
    if (mpeg2 != 0 && decoder->unk13C == 0) {
        mb_height = ((decoder->vertical_size + 31) >> 5) * 2;
    } else {
        mb_height = (decoder->vertical_size + 15) >> 4;
    }
    decoder->mb_height = mb_height;
    height = mb_height << 4;
    width = decoder->mb_width << 4;
    if (width == mpeg->width) {
        if (height == mpeg->height) {
            return;
        }
        field5 = (u8 *)decoder + 0x528;
    } else {
        field5 = (u8 *)decoder + 0x528;
    }
    mpeg->height = height;
    mpeg->width = width;
    frame_size = (u32)(width * 0x180 * height) >> 8;
    pool = (u8 *)decoder + 0x108;
    frame0 = (u8 *)decoder + 0x1E8;
    frame1 = (u8 *)decoder + 0x250;
    frame2 = (u8 *)decoder + 0x2B8;
    field0 = (u8 *)decoder + 0x320;
    field1 = (u8 *)decoder + 0x388;
    field2 = (u8 *)decoder + 0x3F0;
    field3 = (u8 *)decoder + 0x458;
    field4 = (u8 *)decoder + 0x4C0;
    copy_object_field_backward(pool);
    decoder->frame_buffers[0] = reserve_aligned_buffer_space(decoder, pool, frame_size, 0x40);
    decoder->frame_buffers[1] = reserve_aligned_buffer_space(decoder, pool, frame_size, 0x40);
    decoder->frame_buffers[2] = reserve_aligned_buffer_space(decoder, pool, frame_size, 0x40);
    _initRefImages(frame0, frame1, frame2, field0, field1, field2, field3, field4, field5,
                   decoder->frame_buffers[0], decoder->frame_buffers[1], decoder->frame_buffers[2],
                   width, height);
    InitializeReferenceImage(frame0, width, height);
    InitializeReferenceImage(frame1, width, height);
    InitializeReferenceImage(frame2, width, height);
    InitializeReferenceImage(field0, width, height >> 1);
    InitializeReferenceImage(field1, width, height >> 1);
    InitializeReferenceImage(field2, width, height >> 1);
    InitializeReferenceImage(field3, width, height >> 1);
    InitializeReferenceImage(field4, width, height >> 1);
    InitializeReferenceImage(field5, width, height >> 1);
}
#endif /* NON_MATCHING */
