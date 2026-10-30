#include "types.h"
#include "rnc/sdk/libmpeg.h"

s32 sceMpegIsEnd(struct sceMpeg *mpeg) __asm__("func_0012BA48");

s32 sceMpegIsEnd(struct sceMpeg *mpeg) {
    return mpeg->sys->is_end;
}
