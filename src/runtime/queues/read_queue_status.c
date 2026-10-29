#include "types.h"
#include "sda.h"

extern s32 D_0012FC9C NOT_SDA;
extern void SignalSema(s32 semaphore_id);

void ReadQueueStatus(void) {
    SignalSema(D_0012FC9C);
}
