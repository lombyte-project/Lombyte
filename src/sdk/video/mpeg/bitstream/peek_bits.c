#include "types.h"

s32 peek_bits(u64 *value, s32 bits) __asm__("SignExtendPackedValue");

s32 peek_bits(u64 *value, s32 bits) {
    return *value >> (64 - bits);
}
