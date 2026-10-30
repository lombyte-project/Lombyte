#include "types.h"
#include "rnc/sdk/libmpeg.h"

extern u8  image_buffer_alignment_error_format[] __asm__("D_00153A58");
extern s32 _Error1();
extern s32 _decodeOrSkip();
extern s32 _nextHeader();
extern s32 _sceMpegFlush();

s32 _getpic(struct sceMpeg *mpeg) {
    enum {
        MPEG_SEQUENCE_END = 0,
        MPEG_INTRA_PICTURE = 1,
        MPEG_PREDICTIVE_PICTURE = 2,
        MPEG_BIDIRECTIONAL_PICTURE = 3,
        MPEG_DC_INTRA_PICTURE = 4
    };
    s32                 image_addr;
    s32                 status;
    u32                 start_code;
    struct MpegDecoder *decoder;

    start_code = 1;
    status = 0;
    decoder = mpeg->sys;
    image_addr = decoder->unkD8;
    decoder->is_end = 0;
    if (image_addr & 0x3F) {
        _Error1(decoder, image_buffer_alignment_error_format, image_addr);
        return -1;
    }
    decoder->picture_ready = 0;
    do {
        if (status == -1) {
            goto dispatch;
        }
        do {
            start_code = _nextHeader(decoder);
            if (start_code == 0) {
                break;
            }
            if (decoder->picture_structure == decoder->first_field_structure) {
                break;
            }
        } while (decoder->mpeg2 != 0);
    dispatch:
        switch (start_code) {
        case MPEG_SEQUENCE_END:
            _sceMpegFlush(mpeg);
            decoder->is_end = 1;
            break;
        case MPEG_INTRA_PICTURE:
            decoder->picture_count_b = 0;
            decoder->picture_count_p = 0;
            decoder->picture_count_i = 0;
            status = _decodeOrSkip(mpeg, 0, decoder->decode_count_i);
            decoder->picture_count_i = (s32)(decoder->picture_count_i + 1);
            break;
        case MPEG_PREDICTIVE_PICTURE:
            status = _decodeOrSkip(mpeg, decoder->picture_count_p, decoder->decode_count_p);
            decoder->picture_count_p = (s32)(decoder->picture_count_p + 1);
            break;
        case MPEG_BIDIRECTIONAL_PICTURE:
        case MPEG_DC_INTRA_PICTURE:
            status = _decodeOrSkip(mpeg, decoder->picture_count_b, decoder->decode_count_b);
            decoder->picture_count_b = (s32)(decoder->picture_count_b + 1);
            break;
        }
        if (decoder->picture_ready != 0) {
            return 1;
        }
    } while (decoder->is_end == 0);
    return 1;
}
