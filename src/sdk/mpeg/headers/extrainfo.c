#include "rnc/sdk/mpeg/bitstream/_flushBuf.h"
#include "rnc/sdk/mpeg/bitstream/nextbit.h"

void ExtraInfo(void *bitstream) __asm__("_extrainfo");

void ExtraInfo(void *bitstream) {
    while (_nextBit(bitstream, 1) != 0) {
        _flushBuf(bitstream, 8);
    }
}
