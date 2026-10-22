#include "types.h"
typedef struct {
    u8 pad0[0x12C];
    s32 mbWidth;
    s32 mbHeight;
    u8 pad134[0x40];
    s32 pictStruct;
    u8 pad178[0x698];
    s32 unk810;
    s32 unk814;
} MpegDec;

extern char D_001537E0[];
extern s32 _slice0(MpegDec *d, s32 mbs);
extern void _waitIpuIdle(MpegDec *d);
extern s32 _waitBdecOut(MpegDec *d);
extern void _doMC(MpegDec *d, s32 flag);
extern void _Error(MpegDec *d, char *msg);

s32 _pictureData0(MpegDec *decoder) {
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
    } while (decode_status == 1 || decode_status == 3);
    _waitIpuIdle(decoder);
    if (_waitBdecOut(decoder) == 0) {
        decode_status = 2;
    }
    while ((*(volatile u32 *)0x1000D400 >> 8) & 1) {
    }
    if (decode_status == 0) {
        _doMC(decoder, decoder->unk810 == 0);
    }
    if (decode_status == 1 || decode_status == 2) {
        _Error(decoder, D_001537E0);
    }
    return decode_status == 0;
}
