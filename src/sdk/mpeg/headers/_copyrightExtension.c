#include "types.h"
#include "rnc/sdk/mpeg/bitstream.h"

void _copyrightExtension(s32 decoder) {
    _nextBit(decoder, 1);
    _nextBit(decoder, 8);
    _nextBit(decoder, 1);
    _nextBit(decoder, 7);
    _nextBit(decoder, 1);
    _nextBit(decoder, 0x14);
    _nextBit(decoder, 1);
    _nextBit(decoder, 0x16);
    _nextBit(decoder, 1);
    _nextBit(decoder, 0x16);
}
