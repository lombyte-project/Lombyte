#include "types.h"

u32 TimeToBcd(u32 value) {
    value &= 0xFF;
    return ((value / 10) * 6 + value) & 0xFF;
}
