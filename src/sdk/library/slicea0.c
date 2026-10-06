#include "types.h"

struct MpegSliceDecoder {
    u8 reserved_000[0x11C];
    s32 error;
    u8 reserved_120[0xC];
    s32 macroblock_width;
    u8 reserved_130[0x80];
    s32 slice_active;
};

struct MpegMotionState {
    s32 values[8];
};

extern u8 D_00153800[];
extern u8 D_00153828[];
extern s32 _Error();
extern s32 _Error1();
extern s32 _flushBuf();
extern s32 _mbAddressIncrement();
extern s32 _nextStartCode();
extern s32 _peepBit();
extern s32 _sliceB();

s32 _sliceA0(struct MpegSliceDecoder *decoder, s32 unused, s32 *macroblock_address, s32 *increment,
             volatile struct MpegMotionState *motion) {
    s32 slice_extension;
    s32 start_code;
    s32 address_increment;

    decoder->error = 0;
    _nextStartCode();
    start_code = _peepBit(decoder, 0x20);
    if ((u32)(start_code - 0x101) >= 0xAFU) {
        _Error1(decoder, D_00153800, start_code);
        return 2;
    }
    _flushBuf(decoder, 0x20);
    slice_extension = _sliceB(decoder);
    address_increment = _mbAddressIncrement(decoder);
    *increment = address_increment;
    if (decoder->error != 0) {
        _Error(decoder, D_00153828, address_increment);
        return 1;
    }
    *macroblock_address =
        (((((slice_extension << 7) + (start_code & 0xFF)) - 1) * decoder->macroblock_width) +
         address_increment) -
        1;
    *increment = 1;
    decoder->slice_active = 1;
    motion->values[2] = 0;
    motion->values[5] = 0;
    motion->values[4] = 0;
    motion->values[1] = 0;
    motion->values[0] = 0;
    motion->values[7] = 0;
    motion->values[6] = 0;
    motion->values[3] = 0;
    return 0;
}
