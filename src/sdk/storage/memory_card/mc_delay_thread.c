extern int  GetThreadId(void);
extern int  SetAlarm(int delay, void *callback, int thread_id);
extern void SleepThread(void);
extern void
wake_thread_alarm(int alarm_id, unsigned int scheduled_time, int thread_id) __asm__("FUN_00123bc0");

void mcDelayThread(int delay) {
    int   delay_ticks = delay & 0xFFFF;
    void *alarm_callback = wake_thread_alarm;
    int   thread_id = GetThreadId();

    SetAlarm(delay_ticks, alarm_callback, thread_id);
    SleepThread();
}
