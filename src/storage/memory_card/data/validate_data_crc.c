#include "types.h"
extern s32 calculate_crc16() __asm__("func_0020ACC0");
s32 validate_data_crc(s32 *data) __asm__("FUN_0020ad38");

s32 validate_data_crc(s32 *data) {
    s32 ret = 0;
    s32 n = data[1];
    s32 k = data[0];
    if (n != 0) {
        ret = calculate_crc16(data + 2, k) == n;
    }
    return ret;
}

extern __typeof__(validate_data_crc) func_0020AD38 __attribute__((alias("FUN_0020ad38")));
