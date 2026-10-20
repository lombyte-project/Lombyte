#include "types.h"
u16 calculate_crc16(u8 *p, s32 len) __asm__("FUN_0020acc0");

u16 calculate_crc16(u8 *data_cursor, s32 len) {
    u8 *end;
    u32 crc;
    s32 i;

    if (len > 0x1800) {
        return 0;
    }
    end = data_cursor + len;
    crc = 0xEDB88320;
    while (data_cursor < end) {
        crc ^= *data_cursor++ << 8;
        for (i = 7; i >= 0; i--) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1F45;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

extern __typeof__(calculate_crc16) func_0020ACC0 __attribute__((alias("FUN_0020acc0")));
