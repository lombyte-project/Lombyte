#include "rnc/sdk/library/sif_file_slot.h"
#include "rnc/sdk/storage/fileio/scefsseminit.h"

extern s32 WaitSema();

s32 _sceFsWaitS(void) {
    _sceFsSemInit();
    WaitSema(fio_wait_sema);
    return 0;
}
