#include "types.h"
#include "rnc/sdk/system/sysbit_flush.h"

extern s32 peek_bits() __asm__("SignExtendPackedValue");

s32 _sysbitGet(s32 bitstream, s32 flush_flags) {
    s32 decoded_value;

    decoded_value = peek_bits();
    _sysbitFlush(bitstream, flush_flags);
    return decoded_value;
}
