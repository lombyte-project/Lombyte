#ifndef LOMBYTE_RNC_SDK_MPEG_HEADERS_H
#define LOMBYTE_RNC_SDK_MPEG_HEADERS_H

#include "types.h"

struct MpegDecoder;
struct sceMpeg;

void _extensionAndUserData(void *);
void ExtraInfo(void *) __asm__("_extrainfo");
s32  _nextHeader();
void _setDefaultQM(struct MpegDecoder *, s32, u32);
void func_0012C4C8(struct sceMpeg *);

#endif /* LOMBYTE_RNC_SDK_MPEG_HEADERS_H */
