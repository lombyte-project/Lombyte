#include "types.h"

u8 BcdToTime(u8 bcd) {
    u8 correction = (bcd >> 4) * 6;

    return bcd - correction;
}
