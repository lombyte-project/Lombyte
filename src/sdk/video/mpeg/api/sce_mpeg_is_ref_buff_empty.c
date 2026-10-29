#include "types.h"
#include "rnc/sdk/libmpeg.h"

s32 sceMpegIsRefBuffEmpty(struct sceMpeg *mpeg) __asm__("func_0012BA58");

s32 sceMpegIsRefBuffEmpty(struct sceMpeg *mpeg) {
    return mpeg->sys->decoded_pictures == 0;
}
