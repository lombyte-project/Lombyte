#include "rnc/sdk/mpeg/bitstream/peek_bits.h"
#include "rnc/sdk/system/sysbit_flush.h"

int SysbitMarker(void *value) __asm__("_sysbitMarker");

int SysbitMarker(void *bitstream) {
    int marker_bit = peek_bits(bitstream, 1);
    _sysbitFlush(bitstream, 1);
    return marker_bit;
}
