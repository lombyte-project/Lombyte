#ifndef LOMBYTE_RNC_SDK_LIBCDVD_H
#define LOMBYTE_RNC_SDK_LIBCDVD_H

#include "types.h"

/* Read mode passed to sceCdRead: retry count, spindle control and the
   sector data pattern. */
typedef struct sceCdRMode {
    u8 trycount;
    u8 spindlctrl;
    u8 datapattern;
    u8 pad;
} sceCdRMode;

#endif /* LOMBYTE_RNC_SDK_LIBCDVD_H */
