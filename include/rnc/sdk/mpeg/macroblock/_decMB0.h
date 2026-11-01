#ifndef LOMBYTE_RNC_SDK_MPEG_MACROBLOCK__DECMB0_H
#define LOMBYTE_RNC_SDK_MPEG_MACROBLOCK__DECMB0_H

#include "types.h"

struct MpegDecoder;
struct MpegMbState;

int _decMB0(struct MpegDecoder *, int *, int *, int *, struct MpegMbState *, int *, int);

#endif /* LOMBYTE_RNC_SDK_MPEG_MACROBLOCK__DECMB0_H */
