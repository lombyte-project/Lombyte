#include "types.h"
#include "rnc/sdk/video/mpeg/bitstream/peek_bits.h"

extern s32 ReadSysbit(void *state, s32 bit_count) __asm__("_sysbitGet");

s32 ParseSystemHeader(void *state) __asm__("_system_header");

s32 ParseSystemHeader(void *state) {
    ReadSysbit(state, 0x38);
    ReadSysbit(state, 0x28);
    while (peek_bits(state, 1) == 1) {
        ReadSysbit(state, 0x18);
    }
    return 1;
}
