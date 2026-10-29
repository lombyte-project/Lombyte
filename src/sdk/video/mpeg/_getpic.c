#include "types.h"
#include "rnc/sdk/libmpeg.h"

extern u8 image_buffer_alignment_error_format[] __asm__("D_00153A58");
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
    s32 image_addr;
    s32 status;
    u32 start_code;
    struct MpegDecoder *decoder;

    start_code = 1;
    status = 0;
    decoder = mpeg->sys;
    image_addr = decoder->unkD8;
    decoder->unk0 = 0;
    if (image_addr & 0x3F) {
        _Error1(decoder, image_buffer_alignment_error_format, image_addr);
        return -1;
    }
    decoder->unk820 = 0;
    do {
        if (status == -1) {
            goto dispatch;
        }
        do {
            start_code = _nextHeader(decoder);
            if (start_code == 0) {
                break;
            }
            if (decoder->picture_structure == decoder->unkD4) {
                break;
            }
        } while (decoder->mpeg2 != 0);
    dispatch:
        switch (start_code) {
        case MPEG_SEQUENCE_END:
            _sceMpegFlush(mpeg);
            decoder->unk0 = 1;
            break;
        case MPEG_INTRA_PICTURE:
            decoder->unkA8 = 0;
            decoder->unkA4 = 0;
            decoder->unkA0 = 0;
            status = _decodeOrSkip(mpeg, 0, decoder->unk94);
            decoder->unkA0 = (s32)(decoder->unkA0 + 1);
            break;
        case MPEG_PREDICTIVE_PICTURE:
            status = _decodeOrSkip(mpeg, decoder->unkA4, decoder->unk98);
            decoder->unkA4 = (s32)(decoder->unkA4 + 1);
            break;
        case MPEG_BIDIRECTIONAL_PICTURE:
        case MPEG_DC_INTRA_PICTURE:
            status = _decodeOrSkip(mpeg, decoder->unkA8, decoder->unk9C);
            decoder->unkA8 = (s32)(decoder->unkA8 + 1);
            break;
        }
        if (decoder->unk820 != 0) {
            return 1;
        }
    } while (decoder->unk0 == 0);
    return 1;
}
