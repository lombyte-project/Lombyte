#include "rnc/sdk/video/mpeg/bitstream/peek_bits.h"

extern void SysbitFlush(void *value, int count) __asm__("_sysbitFlush");

int SysbitMarker(void *value) __asm__("_sysbitMarker");

int SysbitMarker(void *value) {
    int result = peek_bits(value, 1);
    SysbitFlush(value, 1);
    return result;
}
