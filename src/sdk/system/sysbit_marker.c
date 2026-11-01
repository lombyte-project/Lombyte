#include "rnc/sdk/mpeg/bitstream.h"
#include "rnc/sdk/system.h"

int SysbitMarker(void *value) __asm__("_sysbitMarker");

int SysbitMarker(void *bitstream) {
    int marker_bit = peek_bits(bitstream, 1);
    _sysbitFlush(bitstream, 1);
    return marker_bit;
}
