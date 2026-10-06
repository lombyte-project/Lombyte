/* BcdToTime exact recovery: retail uses the classic `mult`+`mflo` sequence for
   `(temp >> 4) * 6`; the patched public 991111 cc1 (classic-mult patch) emits
   mult/mflo and matches all 8 instructions byte-for-byte.  The frozen
   SN/EE-GCC 2.9 trees emit the R5900 rd-form mult `mult $3,$3,$4` here. */

#include "types.h"

s32 BcdToTime(s32 bcd) {
    u32 low_byte;

    low_byte = bcd & 0xFF;
    return (low_byte - ((low_byte >> 4) * 6)) & 0xFF;
}
