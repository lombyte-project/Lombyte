#include "types.h"

extern s32 D_0012FC9C __attribute__((section(".data")));
extern void SignalSema(s32 semaphore_id);

void ReadQueueStatus(void) {
    SignalSema(D_0012FC9C);
}
