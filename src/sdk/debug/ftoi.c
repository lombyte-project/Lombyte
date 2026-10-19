#include "types.h"

s32 ftoi(u64 bits) {
    s64 exponent;

    exponent = (bits << 1) >> 53;
    exponent -= 1075;
    if (exponent < -53) {
        return 0;
    }
    if (exponent >= 13) {
        return 9999;
    }
    bits = (bits << 12) >> 12;
    bits |= (u64)1 << 52;
    if (exponent < 0) {
        exponent = -exponent;
        bits >>= exponent - 2;
        if ((bits & 3) == 3) {
            bits = (bits >> 2) + 1;
        } else {
            bits >>= 2;
        }
    } else {
        bits <<= exponent;
    }
    return bits;
}
