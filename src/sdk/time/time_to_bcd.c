#include "types.h"

u32 TimeToBcd(u32 binary_value) {
    binary_value &= 0xFF;
    return ((binary_value / 10) * 6 + binary_value) & 0xFF;
}
