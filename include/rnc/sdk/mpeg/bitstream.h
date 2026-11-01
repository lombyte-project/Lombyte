#ifndef LOMBYTE_RNC_SDK_MPEG_BITSTREAM_H
#define LOMBYTE_RNC_SDK_MPEG_BITSTREAM_H

#include "types.h"

void _flushBuf(s32 *, s32);
s32  _peepBit(s32 *, s32);
s32  _nextBit();
void _nextStartCode(s32 *);
s32  peek_bits(void *, s32) __asm__("SignExtendPackedValue");

#endif /* LOMBYTE_RNC_SDK_MPEG_BITSTREAM_H */
