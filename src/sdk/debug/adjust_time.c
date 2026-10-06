/* adjust_time: deci clock adjust; v0-pinned hour with void addhour(). */

#include "types.h"

struct AdjustTimeArg {
    u8 pad_0[2];
    u8 hour;
};

extern void decode_bcd_time_fields() __asm__("func_0012D3C0");
extern void subhour();
extern void addhour();
extern void encode_bcd_time_fields() __asm__("func_0012D428");

void AdjustTime(struct AdjustTimeArg *time, s32 offset) {
    s32 hour;

    decode_bcd_time_fields();
    hour = time->hour + offset;
    if (hour >= 0) {
        if (hour >= 0x3D) {
            do {
                addhour(time);
                hour -= 0x3C;
            } while (hour >= 0x3D);
        }
    } else {
        do {
            hour += 0x3C;
            subhour(time);
        } while (hour < 0);
    }
    time->hour = (u8)hour;
    encode_bcd_time_fields(time);
}
