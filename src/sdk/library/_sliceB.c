#include "types.h"
#include "rnc/sdk/libmpeg.h"
extern s32 _extrainfo();
extern s32 _flushBuf();
extern s32 _nextBit();
s32 _sliceB(struct MpegDecoder *mpeg) {
    mpeg->quantiser_scale_code = _nextBit(mpeg, 5);
    if (_nextBit(mpeg, 1) != 0) {
        _nextBit(mpeg, 1);
        _flushBuf(mpeg, 7);
        _extrainfo(mpeg);
    }
    return 0;
}
