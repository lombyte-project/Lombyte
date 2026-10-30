#include "types.h"

typedef struct {
    s32 type;
    s64 pts;
    s64 dts;
} MpegCbTimeStamp;

typedef struct {
    u8    pad0[0x150];
    s32   unk150;
    u8    pad154[0x6D4];
    s64   pts;
    s64   dts;
    u8    pad838[0x20];
    void *cbData;
} MpegDec;

extern void _nextStartCode(MpegDec *d);
extern u32  _nextBit(MpegDec *d, s32 n);
extern void _sequenceHeader(MpegDec *d);
extern void _groupOfPicturesHeader(MpegDec *d);
extern void _pictureHeader(MpegDec *d);
extern void _dispatchMpegCallback(void *cb, void *arg);

s32 _nextHeader(MpegDec *decoder) {
    enum {
        MPEG_PICTURE_START_CODE = 0x100,
        MPEG_SEQUENCE_HEADER_CODE = 0x1B3,
        MPEG_SEQUENCE_END_CODE = 0x1B7,
        MPEG_GROUP_START_CODE = 0x1B8
    };
    MpegCbTimeStamp cb;
    u32             code;

    while (1) {
        _nextStartCode(decoder);
        code = _nextBit(decoder, 32);
        switch (code) {
        case MPEG_SEQUENCE_HEADER_CODE:
            _sequenceHeader(decoder);
            break;
        case MPEG_GROUP_START_CODE:
            _groupOfPicturesHeader(decoder);
            break;
        case MPEG_PICTURE_START_CODE:
            _pictureHeader(decoder);
            cb.type = 5;
            cb.pts = -1;
            cb.dts = -1;
            _dispatchMpegCallback(decoder->cbData, &cb);
            decoder->dts = cb.dts;
            decoder->pts = cb.pts;
            return decoder->unk150;
        case MPEG_SEQUENCE_END_CODE:
            return 0;
        }
    }
}
