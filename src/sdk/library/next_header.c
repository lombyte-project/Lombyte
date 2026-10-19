#include "types.h"
typedef struct {
    s32 type;
    s64 pts;
    s64 dts;
} MpegCbTimeStamp;

typedef struct {
    u8 pad0[0x150];
    s32 unk150;
    u8 pad154[0x6D4];
    s64 pts;
    s64 dts;
    u8 pad838[0x20];
    void *cbData;
} MpegDec;

extern void _nextStartCode(MpegDec *d);
extern u32 _nextBit(MpegDec *d, s32 n);
extern void _sequenceHeader(MpegDec *d);
extern void _groupOfPicturesHeader(MpegDec *d);
extern void _pictureHeader(MpegDec *d);
extern void _dispatchMpegCallback(void *cb, void *arg);

s32 _nextHeader(MpegDec *d) {
    enum {
        MPEG_PICTURE_START_CODE = 0x100,
        MPEG_SEQUENCE_HEADER_CODE = 0x1B3,
        MPEG_SEQUENCE_END_CODE = 0x1B7,
        MPEG_GROUP_START_CODE = 0x1B8
    };
    MpegCbTimeStamp cb;
    u32 code;

    while (1) {
        _nextStartCode(d);
        code = _nextBit(d, 32);
        switch (code) {
        case MPEG_SEQUENCE_HEADER_CODE:
            _sequenceHeader(d);
            break;
        case MPEG_GROUP_START_CODE:
            _groupOfPicturesHeader(d);
            break;
        case MPEG_PICTURE_START_CODE:
            _pictureHeader(d);
            cb.type = 5;
            cb.pts = -1;
            cb.dts = -1;
            _dispatchMpegCallback(d->cbData, &cb);
            d->dts = cb.dts;
            d->pts = cb.pts;
            return d->unk150;
        case MPEG_SEQUENCE_END_CODE:
            return 0;
        }
    }
}
