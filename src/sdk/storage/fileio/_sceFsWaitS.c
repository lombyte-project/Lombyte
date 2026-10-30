#include "rnc/sdk/library/sif_file_slot.h"

extern s32  WaitSema();
extern void _sceFsSemInit();

s32 _sceFsWaitS(void) {
    _sceFsSemInit();
    WaitSema(fio_wait_sema);
    return 0;
}
