#include "types.h"
struct MpegDecoder {
    u8 pad_0[0x144];
    s32 unk144;
    s32 unk148;
    s32 unk14C;
};

extern s32 _nextBit();
void _sequenceDisplayExtension(struct MpegDecoder *mpeg) {
    _nextBit(mpeg, 3);
    if (_nextBit(mpeg, 1) != 0) {
        _nextBit(mpeg, 8);
        _nextBit(mpeg, 8);
        mpeg->unk144 = _nextBit(mpeg, 8);
    }
    mpeg->unk148 = _nextBit(mpeg, 0xE);
    _nextBit(mpeg, 1);
    mpeg->unk14C = _nextBit(mpeg, 0xE);
}
