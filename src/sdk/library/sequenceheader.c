#include "types.h"
#include "rnc/sdk/libmpeg.h"

extern u8 D_00132FC0[];
extern u8 D_00133000[];
extern u8 D_00153AE8[];
extern s32 _Error();
extern void _extensionAndUserData(struct MpegDecoder *);
extern u32 _nextBit(struct MpegDecoder *, s32);
extern void _sendIpuCommand(struct MpegDecoder *, s32);
extern void _setDefaultQM(struct MpegDecoder *, s32, u32);
extern void _waitIpuIdle(struct MpegDecoder *);
extern void func_0012C4C8(struct sceMpeg *);

void _sequenceHeader(struct MpegDecoder *decoder) {
    u32 bits;
    s32 vertical_size;

    decoder->unkD4 = 0;
    bits = _nextBit(decoder, 32);
    vertical_size = (bits >> 8) & 0xFFF;
    decoder->horizontal_size = bits >> 20;
    decoder->vertical_size = vertical_size;
    if (vertical_size > 0xAF0) {
        _Error(decoder, D_00153AE8);
    }
    bits = _nextBit(decoder, 30);
    decoder->bit_rate = bits >> 12;
    decoder->vbv_buffer_size = (bits >> 1) & 0x3FF;
    decoder->load_intra_quantiser_matrix = _nextBit(decoder, 1);
    if (decoder->load_intra_quantiser_matrix != 0) {
        _waitIpuIdle(decoder);
        _sendIpuCommand(decoder, 0x50000000);
        _waitIpuIdle(decoder);
    } else {
        _setDefaultQM(decoder, 0x50000000, (u32)D_00132FC0);
    }
    decoder->load_non_intra_quantiser_matrix = _nextBit(decoder, 1);
    if (decoder->load_non_intra_quantiser_matrix != 0) {
        _waitIpuIdle(decoder);
        _sendIpuCommand(decoder, 0x58000000);
        _waitIpuIdle(decoder);
    } else {
        _setDefaultQM(decoder, 0x58000000, (u32)D_00133000);
    }
    _extensionAndUserData(decoder);
    func_0012C4C8(decoder->mpeg);
}
