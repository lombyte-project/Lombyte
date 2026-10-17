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
    MpegCbTimeStamp cb;
    u32 code;

    while (1) {
        _nextStartCode(d);
        code = _nextBit(d, 32);
        switch (code) {
        case 0x1B3:
            _sequenceHeader(d);
            break;
        case 0x1B8:
            _groupOfPicturesHeader(d);
            break;
        case 0x100:
            _pictureHeader(d);
            cb.type = 5;
            cb.pts = -1;
            cb.dts = -1;
            _dispatchMpegCallback(d->cbData, &cb);
            d->dts = cb.dts;
            d->pts = cb.pts;
            return d->unk150;
        case 0x1B7:
            return 0;
        }
    }
}
