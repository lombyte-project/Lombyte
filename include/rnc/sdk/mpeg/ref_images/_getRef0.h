#ifndef LOMBYTE_RNC_SDK_MPEG_REF_IMAGES__GETREF0_H
#define LOMBYTE_RNC_SDK_MPEG_REF_IMAGES__GETREF0_H

#include "types.h"

struct MpegDecoder;
struct MpegRefImage;

void _getRef0(
    struct MpegDecoder *, struct MpegRefImage *, int, int, int, int, int, int, int, int, int, int);

#endif /* LOMBYTE_RNC_SDK_MPEG_REF_IMAGES__GETREF0_H */
