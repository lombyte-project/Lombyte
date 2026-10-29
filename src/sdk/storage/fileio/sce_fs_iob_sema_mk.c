#include "types.h"
#include "kernel.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sif_file_slot.h"

extern s32 CreateSema();

void _sceFsIobSemaMK(void) {
    struct SemaParam semaphore_creation_settings;

    if (fio_iob_sema == -1) {
        semaphore_creation_settings.option = 0;
        semaphore_creation_settings.initCount = 1;
        semaphore_creation_settings.maxCount = 1;
        fio_iob_sema = CreateSema(&semaphore_creation_settings);
        fio_async_table_sema = CreateSema(&semaphore_creation_settings);
    }
}
