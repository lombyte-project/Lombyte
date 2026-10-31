#include "rnc/sdk/video/mpeg/bitstream/peek_bits.h"

extern void SysbitFlush(void *value, int count) __asm__("_sysbitFlush");

int SysbitMarker(void *value) __asm__("_sysbitMarker");

int SysbitMarker(void *bitstream) {
    int marker_bit = peek_bits(bitstream, 1);
    SysbitFlush(bitstream, 1);
    return marker_bit;
}
