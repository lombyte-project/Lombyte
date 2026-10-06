#include "types.h"

typedef struct ClockState {
    u8 reserved[3];
    u8 hour;
} ClockState;

extern void NormalizeClock(ClockState *clock_state) __asm__("adddate");

void AddClockHour(ClockState *clock_state) __asm__("addhour");

void AddClockHour(ClockState *clock_state) {
    s32 incremented_hour = clock_state->hour + 1;
    clock_state->hour = (u8)incremented_hour;
    if ((incremented_hour & 0xFF) == 24) {
        clock_state->hour = 0;
        NormalizeClock(clock_state);
    }
}
