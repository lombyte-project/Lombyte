#include "types.h"
#include "rnc/sdk/libmpeg.h"
extern s32 _nextBit();
void _sequenceDisplayExtension(struct MpegDecoder *mpeg) {
    _nextBit(mpeg, 3);
    if (_nextBit(mpeg, 1) != 0) {
        _nextBit(mpeg, 8);
        _nextBit(mpeg, 8);
        mpeg->matrix_coefficients = _nextBit(mpeg, 8);
    }
    mpeg->display_horizontal_size = _nextBit(mpeg, 0xE);
    _nextBit(mpeg, 1);
    mpeg->display_vertical_size = _nextBit(mpeg, 0xE);
}
