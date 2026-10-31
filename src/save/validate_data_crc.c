#include "types.h"

extern s32 calculate_crc16() __asm__("func_0020ACC0");
s32        validate_data_crc(s32 *data) __asm__("FUN_0020ad38");

s32 validate_data_crc(s32 *crc_record) {
    s32 crc_is_valid = 0;
    s32 stored_crc = crc_record[1];
    s32 k = crc_record[0];
    if (stored_crc != 0) {
        crc_is_valid = calculate_crc16(crc_record + 2, k) == stored_crc;
    }
    return crc_is_valid;
}

extern __typeof__(validate_data_crc) func_0020AD38 __attribute__((alias("FUN_0020ad38")));
