#include "types.h"
#include "rnc/sdk/libmpeg.h"
#include "rnc/sdk/mpeg/bitstream/_flushBuf.h"
#include "rnc/sdk/mpeg/bitstream/nextbit.h"
#include "rnc/sdk/mpeg/headers/extrainfo.h"

s32 _sliceB(struct MpegDecoder *mpeg) {
    mpeg->quantiser_scale_code = _nextBit(mpeg, 5);
    if (_nextBit(mpeg, 1) != 0) {
        _nextBit(mpeg, 1);
        _flushBuf(mpeg, 7);
        ExtraInfo(mpeg);
    }
    return 0;
}
