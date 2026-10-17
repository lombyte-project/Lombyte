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
extern void _dispatchMpegCallback(void *FUN_L11_003192c8, void *arg);

s32 _nextHeader(MpegDec *d) {
    MpegCbTimeStamp FUN_L11_003192c8;
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
            FUN_L11_003192c8.type = 5;
            FUN_L11_003192c8.pts = -1;
            FUN_L11_003192c8.dts = -1;
            _dispatchMpegCallback(d->cbData, &FUN_L11_003192c8);
            d->dts = FUN_L11_003192c8.dts;
            d->pts = FUN_L11_003192c8.pts;
            return d->unk150;
        case 0x1B7:
            return 0;
        }
    }
}
