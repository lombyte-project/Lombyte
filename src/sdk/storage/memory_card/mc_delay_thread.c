#include "rnc/sdk/callbacks.h"

extern int  GetThreadId(void);
extern int  SetAlarm(int delay, void *callback, int thread_id);
extern void SleepThread(void);

void mcDelayThread(int delay) {
    int   delay_ticks = delay & 0xFFFF;
    void *alarm_callback = wake_thread_alarm;
    int   thread_id = GetThreadId();

    SetAlarm(delay_ticks, alarm_callback, thread_id);
    SleepThread();
}
