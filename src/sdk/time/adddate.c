#include "types.h"

typedef struct ClockDate {
    u8 reserved[5];
    u8 day;
    u8 month;
    u8 year;
} ClockDate;

typedef struct MonthLengthTable {
    s8 days[12];
} MonthLengthTable;

extern const MonthLengthTable MonthLengths __asm__("D_00153C40");

void AddDate(ClockDate *clock_date) __asm__("adddate");

void AddDate(ClockDate *clock_date) {
    MonthLengthTable month_lengths = MonthLengths;
    s32 next_day = clock_date->day + 1;

    clock_date->day = (u8)next_day;
    if ((*(volatile u8 *)&clock_date->year & 3) == 0) {
        month_lengths.days[1] = 29;
    }
    if (month_lengths.days[clock_date->month - 1] < clock_date->day) {
        clock_date->day = 1;
        clock_date->month++;
        if ((clock_date->month & 0xFF) == 13) {
            if (clock_date->year == 99) {
                clock_date->year = 0;
            } else {
                clock_date->year++;
            }
            clock_date->month = 1;
        }
    }
}
