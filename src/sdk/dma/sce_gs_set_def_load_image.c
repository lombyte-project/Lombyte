#include "types.h"
#include "rnc/rendering/pixel_format.h"
#include "rnc/sdk/libgraph.h"
#include "eetypes.h"
#include "kernel.h"
extern void scePrintf(const char *message, ...);

s32 sceGsSetDefLoadImage(sceGsLoadImage *image, s16 destination_base,
                         s16 destination_width, s16 pixel_format,
                         s16 destination_x, s16 destination_y,
                         s16 width, s16 height) {
    s32 size = 0;

    switch (pixel_format) {
    case PIXEL_FORMAT_32_BIT:
    case PIXEL_FORMAT_32_BIT_ALTERNATE:
        size = (width * height) >> 2;
        break;
    case PIXEL_FORMAT_24_BIT:
    case PIXEL_FORMAT_24_BIT_ALTERNATE:
        size = (width * height * 3) >> 4;
        break;
    case PIXEL_FORMAT_16_BIT:
    case PIXEL_FORMAT_16_BIT_ALTERNATE:
    case PIXEL_FORMAT_16_BIT_ALTERNATE_2:
    case PIXEL_FORMAT_16_BIT_ALTERNATE_3:
        size = (width * height) >> 3;
        break;
    case PIXEL_FORMAT_8_BIT:
    case PIXEL_FORMAT_8_BIT_ALTERNATE:
        size = (width * height) >> 4;
        break;
    case PIXEL_FORMAT_4_BIT:
    case PIXEL_FORMAT_4_BIT_ALTERNATE:
    case PIXEL_FORMAT_4_BIT_ALTERNATE_2:
        size = (width * height) >> 5;
        break;
    }

    if (size > 0x7fff) {
        scePrintf("sceGsSetDefLoadImage: too big size\r\n");
        return 0;
    }

    *(u128 *)&image->q[10] = 0;
    *(u128 *)&image->q[0] = 0;
    ((struct GifTag *)&image->q[0])->NLOOP = 4;
    image->q[0] = (image->q[0] & 0x0fffffffffffffffULL) | 0x1000000000000000ULL;
    ((struct GifTag *)&image->q[0])->REGS0 = 0xe;
    image->q[2] = ((u64)(s64)destination_base << 0x20) |
                  ((u64)(s64)destination_width << 0x30) |
                  ((u64)(s64)pixel_format << 0x38);
    image->q[3] = 0x50;
    image->q[4] = ((u64)(s64)destination_x << 0x20) |
                  ((u64)(s64)destination_y << 0x30);
    image->q[5] = 0x51;
    image->q[6] = (u64)(s64)width | ((u64)(s64)height << 0x20);
    image->q[7] = 0x52;
    image->q[8] = 0;
    image->q[9] = 0x53;
    ((struct GifTag *)&image->q[10])->NLOOP = size;
    ((struct GifTag *)&image->q[10])->EOP = 1;
    ((struct GifTag *)&image->q[10])->FLG = 2;
    CpuSync();
    return 6;
}
