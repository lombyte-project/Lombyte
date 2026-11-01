#ifndef LOMBYTE_RNC_SDK_MPEG_DEMUX__GETPTSDTSFLAGS_H
#define LOMBYTE_RNC_SDK_MPEG_DEMUX__GETPTSDTSFLAGS_H

#include "types.h"

struct MpegPictureTimestampRecord;
struct MpegTimestampState;

void GetMpegPictureTimestampsAndFlags(struct MpegTimestampState *,
                                      struct MpegPictureTimestampRecord *,
                                      s64 *,
                                      s64 *,
                                      s64 *) __asm__("_getPtsDtsFlags");

#endif /* LOMBYTE_RNC_SDK_MPEG_DEMUX__GETPTSDTSFLAGS_H */
