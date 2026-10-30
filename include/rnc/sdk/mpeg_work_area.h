#ifndef LOMBYTE_RNC_SDK_MPEG_WORK_AREA_H
#define LOMBYTE_RNC_SDK_MPEG_WORK_AREA_H

#include "types.h"

/* libmpeg's work area: sceMpegCreate hands it the caller's buffer and the
   decoder takes its frame buffers from it. */
struct MpegWorkArea {
    s32 base;
    s32 size;
    s32 cursor; /* next free byte */
    s32 mark;   /* cursor saved by mark_work_area, restored by reset_work_area */
};

#endif /* LOMBYTE_RNC_SDK_MPEG_WORK_AREA_H */
