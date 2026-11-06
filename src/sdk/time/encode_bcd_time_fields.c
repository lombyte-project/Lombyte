#include "types.h"
#include "rnc/sdk/time.h"

/* sceCdCLOCK in the PS2 SDK <libcdvd.h>. */
struct BcdClockTime {
    u8 stat;
    u8 second;
    u8 minute;
    u8 hour;
    u8 pad;
    u8 day;
    u8 month;
    u8 year;
};

void encode_bcd_time_fields(struct BcdClockTime *time) __asm__("FUN_0012d428");

void encode_bcd_time_fields(struct BcdClockTime *time) {
    time->year = TimeToBcd(time->year);
    time->month = TimeToBcd(time->month);
    time->day = TimeToBcd(time->day);
    time->hour = TimeToBcd(time->hour);
    time->minute = TimeToBcd(time->minute);
    time->second = TimeToBcd(time->second);
}
