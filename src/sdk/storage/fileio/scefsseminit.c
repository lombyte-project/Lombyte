#include "types.h"
#include "kernel.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sif_file_slot.h"

extern s32 CreateSema();

void _sceFsSemInit(void) {
    struct SemaParam file_call_semaphore_config;

    if (fio_wait_sema == -1) {
        file_call_semaphore_config.option = 0;
        file_call_semaphore_config.initCount = 1;
        file_call_semaphore_config.maxCount = 1;
        fio_wait_sema = CreateSema(&file_call_semaphore_config);
    }
}
