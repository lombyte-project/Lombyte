#include "types.h"
#include "rnc/sdk/libmpeg.h"

extern u8  chroma_intra_matrix_error_message[] __asm__("D_001538E8");
const u8   D_00153910[] = "load_chroma_non_intra_quantizer_matrix == 1";
extern s32 _Error();
extern s32 _nextBit();
extern s32 _sendIpuCommand();
extern s32 _waitIpuIdle();

void _quantMatrixExtension(struct MpegDecoder *mpeg) {
    s32 load_non_intra;
    s32 load_intra;

    load_intra = _nextBit(mpeg, 1);
    mpeg->load_intra_quantiser_matrix = load_intra;
    if (load_intra != 0) {
        _waitIpuIdle(mpeg);
        _sendIpuCommand(mpeg, 0x50000000);
        _waitIpuIdle(mpeg);
    }
    load_non_intra = _nextBit(mpeg, 1);
    mpeg->load_non_intra_quantiser_matrix = load_non_intra;
    if (load_non_intra != 0) {
        _waitIpuIdle(mpeg);
        _sendIpuCommand(mpeg, 0x58000000);
        _waitIpuIdle(mpeg);
    }
    if (_nextBit(mpeg, 1) != 0) {
        _Error(mpeg, chroma_intra_matrix_error_message);
    }
    if (_nextBit(mpeg, 1) != 0) {
        _Error(mpeg, D_00153910);
    }
}
