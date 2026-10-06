extern int CreateSema(int *parameters) __asm__("CreateSema");
extern void SetAlarm(int delay, void *callback, int semaphore) __asm__("SetAlarm");
extern void WaitSema(int semaphore) __asm__("WaitSema");
extern void DeleteSema(int semaphore) __asm__("DeleteSema");
extern unsigned char AlarmCallback[1] __asm__("D_001205E8") __attribute__((section(".data")));

void SceCdDelayThread(int delay) __asm__("sceCdDelayThread");

void SceCdDelayThread(int delay) {
    int parameters[6];
    int delay_reg = delay & 0xffff;
    int semaphore;

    parameters[1] = 1;
    parameters[2] = 0;
    parameters[5] = 0;
    semaphore = CreateSema(parameters);
    SetAlarm(delay_reg, AlarmCallback, semaphore);
    WaitSema(semaphore);
    DeleteSema(semaphore);
}
