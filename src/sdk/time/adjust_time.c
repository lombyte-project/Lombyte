/* adjust_time: deci clock adjust; v0-pinned hour with void AddClockHour(). */

#include "types.h"
#include "rnc/sdk/time.h"

struct AdjustTimeArg {
    u8 pad_0[2];
    u8 hour;
};

extern void decode_bcd_time_fields() __asm__("func_0012D3C0");
extern void encode_bcd_time_fields() __asm__("func_0012D428");

void AdjustTime(struct AdjustTimeArg *time, s32 minute_delta) {
    s32 hour;

    decode_bcd_time_fields();
    hour = time->hour + minute_delta;
    if (hour >= 0) {
        if (hour >= 0x3D) {
            do {
                AddClockHour(time);
                hour -= 0x3C;
            } while (hour >= 0x3D);
        }
    } else {
        do {
            hour += 0x3C;
            Subhour(time);
        } while (hour < 0);
    }
    time->hour = (u8)hour;
    encode_bcd_time_fields(time);
}
