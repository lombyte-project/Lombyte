#include "types.h"
#include "rnc/sdk/mpeg/bitstream/peek_bits.h"
#include "rnc/sdk/system/sysbit_get.h"

s32 ParseSystemHeader(void *state) __asm__("_system_header");

s32 ParseSystemHeader(void *state) {
    _sysbitGet(state, 0x38);
    _sysbitGet(state, 0x28);
    while (peek_bits(state, 1) == 1) {
        _sysbitGet(state, 0x18);
    }
    return 1;
}
