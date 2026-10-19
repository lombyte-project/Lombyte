#include "types.h"

s32 SignExtendPackedValue(u64 *value, s32 bits) {
    return *value >> (64 - bits);
}
