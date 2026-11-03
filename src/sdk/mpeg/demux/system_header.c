#include "types.h"
#include "rnc/sdk/mpeg/bitstream.h"
#include "rnc/sdk/system.h"

s32 ParseSystemHeader(void *state) __asm__("_system_header");

s32 ParseSystemHeader(void *bitstream_context) {
    _sysbitGet(bitstream_context, 0x38);
    _sysbitGet(bitstream_context, 0x28);
    while (peek_bits(bitstream_context, 1) == 1) {
        _sysbitGet(bitstream_context, 0x18);
    }
    return 1;
}
