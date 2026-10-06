#include "types.h"
struct MpegDecoder {
    u8 pad_0[0x1B4];
    s32 unk1B4;
};

extern s32 _extrainfo();
extern s32 _flushBuf();
extern s32 _nextBit();
s32 _sliceB(struct MpegDecoder *mpeg) {
    mpeg->unk1B4 = _nextBit(mpeg, 5);
    if (_nextBit(mpeg, 1) != 0) {
        _nextBit(mpeg, 1);
        _flushBuf(mpeg, 7);
        _extrainfo(mpeg);
    }
    return 0;
}
