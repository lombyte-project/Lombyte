#include "types.h"
#include "sda.h"
#include "rnc/sdk/library/sif_file_slot.h"

extern void SignalSema(s32 semaphore_id);

void ReadQueueStatus(void) {
    SignalSema(fio_wait_sema);
}
