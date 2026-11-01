#include "rnc/sdk/storage/cdvd/signal_native_cd_delay_alarm.h"

extern int  CreateSema(int *parameters) __asm__("CreateSema");
extern void SetAlarm(int delay, void *callback, int semaphore) __asm__("SetAlarm");
extern void WaitSema(int semaphore) __asm__("WaitSema");
extern void DeleteSema(int semaphore) __asm__("DeleteSema");

void SceCdDelayThread(int delay) __asm__("sceCdDelayThread");

void SceCdDelayThread(int delay) {
    int parameters[6];
    int delay_reg = delay & 0xffff;
    int semaphore;

    parameters[1] = 1;
    parameters[2] = 0;
    parameters[5] = 0;
    semaphore = CreateSema(parameters);
    SetAlarm(delay_reg, signal_native_cd_delay_alarm, semaphore);
    WaitSema(semaphore);
    DeleteSema(semaphore);
}
