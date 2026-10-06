#include "types.h"
extern s32 calculate_crc16() __asm__("func_0020ACC0");
s32 validate_data_crc(s32 *arg0) __asm__("FUN_0020ad38");

s32 validate_data_crc(s32 *arg0) {
    s32 ret = 0;
    s32 n = arg0[1];
    s32 k = arg0[0];
    if (n != 0) {
        ret = calculate_crc16(arg0 + 2, k) == n;
    }
    return ret;
}

extern __typeof__(validate_data_crc) func_0020AD38 __attribute__((alias("FUN_0020ad38")));
