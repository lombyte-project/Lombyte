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

void decode_bcd_time_fields(struct BcdClockTime *time) __asm__("FUN_0012d3c0");

void decode_bcd_time_fields(struct BcdClockTime *time) {
    time->year = BcdToTime(time->year);
    time->month = BcdToTime(time->month);
    time->day = BcdToTime(time->day);
    time->hour = BcdToTime(time->hour);
    time->minute = BcdToTime(time->minute);
    time->second = BcdToTime(time->second);
}

extern void func_0012D3C0(struct BcdClockTime *) __attribute__((alias("FUN_0012d3c0")));
