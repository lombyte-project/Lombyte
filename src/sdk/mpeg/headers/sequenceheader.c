#include "types.h"
#include "rnc/sdk/libmpeg.h"
#include "rnc/sdk/ipu.h"
#include "rnc/sdk/mpeg/bitstream.h"
#include "rnc/sdk/mpeg/errors.h"
#include "rnc/sdk/mpeg/headers.h"

extern u8  default_intra_quantiser_matrix[] __asm__("D_00132FC0");
extern u8  default_non_intra_quantiser_matrix[] __asm__("D_00133000");
const char vertical_size_limit_error_message[] __asm__("D_00153AE8") = "vertical size > 2800";

void _sequenceHeader(struct MpegDecoder *decoder) {
    u32 bits;
    s32 vertical_size;

    decoder->first_field_structure = 0;
    bits = _nextBit(decoder, 32);
    vertical_size = (bits >> 8) & 0xFFF;
    decoder->horizontal_size = bits >> 20;
    decoder->vertical_size = vertical_size;
    if (vertical_size > 0xAF0) {
        _Error(decoder, vertical_size_limit_error_message);
    }
    bits = _nextBit(decoder, 30);
    decoder->bit_rate = bits >> 12;
    decoder->vbv_buffer_size = (bits >> 1) & 0x3FF;
    decoder->load_intra_quantiser_matrix = _nextBit(decoder, 1);
    if (decoder->load_intra_quantiser_matrix != 0) {
        _waitIpuIdle(decoder);
        SendIpuCommand(decoder, 0x50000000);
        _waitIpuIdle(decoder);
    } else {
        _setDefaultQM(decoder, 0x50000000, (u32)default_intra_quantiser_matrix);
    }
    decoder->load_non_intra_quantiser_matrix = _nextBit(decoder, 1);
    if (decoder->load_non_intra_quantiser_matrix != 0) {
        _waitIpuIdle(decoder);
        SendIpuCommand(decoder, 0x58000000);
        _waitIpuIdle(decoder);
    } else {
        _setDefaultQM(decoder, 0x58000000, (u32)default_non_intra_quantiser_matrix);
    }
    _extensionAndUserData(decoder);
    func_0012C4C8(decoder->mpeg);
}
