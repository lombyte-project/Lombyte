#ifndef LOMBYTE_RNC_SDK_MPEG_API_DISPATCH_MPEG_CALLBACK_H
#define LOMBYTE_RNC_SDK_MPEG_API_DISPATCH_MPEG_CALLBACK_H

#include "types.h"

struct MpegContext;
struct MpegEvent;

s32 _dispatchMpegCallback(struct MpegContext *, struct MpegEvent *);

#endif /* LOMBYTE_RNC_SDK_MPEG_API_DISPATCH_MPEG_CALLBACK_H */
