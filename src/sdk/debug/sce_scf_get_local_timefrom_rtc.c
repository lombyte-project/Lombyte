#include "types.h"
extern s32 sceScfGetSummerTime();
extern s32 sceScfGetTimeZone();
void sceScfGetLocalTimefromRTC(s32 rtc_time) {
    register s32 rtc_time_s0;
    s32 time_zone;
    s32 summer_minutes;

    rtc_time_s0 = rtc_time;
    time_zone = sceScfGetTimeZone();
    summer_minutes = sceScfGetSummerTime() * 0x3C;
    summer_minutes -= 0x21C;
    AdjustTime(rtc_time_s0, time_zone + summer_minutes);
}
