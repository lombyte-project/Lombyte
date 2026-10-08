#ifndef LOMBYTE_RNC_SDK_LIBRARY_SIF_FILE_SLOT_H
#define LOMBYTE_RNC_SDK_LIBRARY_SIF_FILE_SLOT_H

#include "types.h"

/* One open-file slot of the SIF file I/O wrappers. */
struct SifFileSlot {
    s32 fd;
    s32 flags;
    s32 reserved8;
    s32 reservedC;
};

#endif /* LOMBYTE_RNC_SDK_LIBRARY_SIF_FILE_SLOT_H */
