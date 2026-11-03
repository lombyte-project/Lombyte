#include "types.h"
#include "rnc/sdk/ipu.h"
#include "rnc/sdk/mpeg/errors.h"
#include "rnc/sdk/mpeg/macroblock.h"

typedef struct {
    u8  pad0[0x12C];
    s32 mbWidth;
    s32 mbHeight;
    u8  pad134[0x40];
    s32 pictStruct;
    u8  pad178[0x698];
    s32 unk810;
    s32 unk814;
} MpegDec;

const char
    skip_to_next_picture_error_message[] __asm__("D_001537E0") = "= Skip to the next picture =";

s32 _pictureData0(MpegDec *decoder) {
    enum {
        DECODE_STATUS_SUCCESS = 0,
        DECODE_STATUS_ABORT_PICTURE = 2
    };
    enum {
        DECODE_STATUS_SLICE_DECODE_ERROR = 1
    };
    enum {
        DECODE_STATUS_SLICE_LOOKAHEAD_STOP = 3
    };
    s32 decode_status;
    s32 mbs;

    decoder->unk810 = 0;
    decoder->unk814 = 0;
    mbs = decoder->mbWidth * decoder->mbHeight;
    if (decoder->pictStruct != 3) {
        mbs >>= 1;
    }
    do {
        decode_status = _slice0(decoder, mbs);
    } while (decode_status == DECODE_STATUS_SLICE_DECODE_ERROR ||
             decode_status == DECODE_STATUS_SLICE_LOOKAHEAD_STOP);
    _waitIpuIdle(decoder);
    if (_waitBdecOut(decoder) == 0) {
        decode_status = DECODE_STATUS_ABORT_PICTURE;
    }
    while ((*(volatile u32 *)0x1000D400 >> 8) & 1) {
    }
    if (decode_status == DECODE_STATUS_SUCCESS) {
        _doMC(decoder, decoder->unk810 == 0);
    }
    if (decode_status == DECODE_STATUS_SLICE_DECODE_ERROR ||
        decode_status == DECODE_STATUS_ABORT_PICTURE) {
        _Error(decoder, skip_to_next_picture_error_message);
    }
    return decode_status == DECODE_STATUS_SUCCESS;
}
