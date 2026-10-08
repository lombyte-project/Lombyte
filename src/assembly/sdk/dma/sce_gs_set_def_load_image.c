#include "types.h"
#include "eetypes.h"
#include "asm.h"

#ifndef NON_MATCHING
/* Exact SDK/library unit sceGsSetDefLoadImage; symbolic expected assembly retained pending source recovery. */
INCLUDE_ASM(
    "config/us/expected/asm/assembly/sdk/dma/sce_gs_set_def_load_image/sceGsSetDefLoadImage.s",
    sceGsSetDefLoadImage);
#else
#include "types.h"
#include "rnc/sdk/libgraph.h"
#include "eetypes.h"
extern void scePrintf(const char *message, ...);
extern void __sync_synchronize(void);

s32 sceGsSetDefLoadImage(sceGsLoadImage *image, s16 destination_base,
                         s16 destination_width, s16 pixel_format,
                         s16 destination_x, s16 destination_y,
                         s16 width, s16 height) {
    s32 size = 0;

    switch (pixel_format) {
    case 0:
    case 0x30:
        size = (width * height) >> 2;
        break;
    case 1:
    case 0x31:
        size = (width * height * 3) >> 4;
        break;
    case 2:
    case 10:
    case 0x32:
    case 0x3a:
        size = (width * height) >> 3;
        break;
    case 0x13:
    case 0x1b:
        size = (width * height) >> 4;
        break;
    case 0x14:
    case 0x24:
    case 0x2c:
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
    ((struct GifTag *)&image->q[0])->NREG = 1;
    image->q[1] = (image->q[1] & ~0xfULL) | 0xe;
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
    __sync_synchronize();
    return 6;
}
#endif /* NON_MATCHING */
