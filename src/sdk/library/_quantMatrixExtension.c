#include "types.h"
struct MpegDecoder {
    u8 pad_0[0x840];
    s32 unk840;
    s32 unk844;
};

extern u8 D_001538E8[];
extern u8 D_00153910[];
extern s32 _Error();
extern s32 _nextBit();
extern s32 _sendIpuCommand();
extern s32 _waitIpuIdle();
void _quantMatrixExtension(struct MpegDecoder *mpeg) {
    s32 temp_2_20;
    s32 temp_2_7;

    temp_2_7 = _nextBit(mpeg, 1);
    mpeg->unk840 = temp_2_7;
    if (temp_2_7 != 0) {
        _waitIpuIdle(mpeg);
        _sendIpuCommand(mpeg, 0x50000000);
        _waitIpuIdle(mpeg);
    }
    temp_2_20 = _nextBit(mpeg, 1);
    mpeg->unk844 = temp_2_20;
    if (temp_2_20 != 0) {
        _waitIpuIdle(mpeg);
        _sendIpuCommand(mpeg, 0x58000000);
        _waitIpuIdle(mpeg);
    }
    if (_nextBit(mpeg, 1) != 0) {
        _Error(mpeg, D_001538E8);
    }
    if (_nextBit(mpeg, 1) != 0) {
        _Error(mpeg, D_00153910);
    }
}
