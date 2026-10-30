#include "types.h"
#include "kernel.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/library/sif_file_slot.h"

extern s32 CreateSema();

void _sceFsIobSemaMK(void) {
    struct SemaParam args;

    if (fio_iob_sema == -1) {
        args.option = 0;
        args.initCount = 1;
        args.maxCount = 1;
        fio_iob_sema = CreateSema(&args);
        fio_async_table_sema = CreateSema(&args);
    }
}
