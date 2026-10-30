#include "types.h"
#include "rnc/sdk/sce_gs_gparam.h"
#include "rnc/sdk/graph/sce_gs_get_gparam.h"

s16 sceGszbufaddr(s16 psm, s16 width_pixels, s16 height_pixels) {
    sceGsGParam *gp;
    s32          fw;
    s32          fh;

    gp = sceGsGetGParam();
    fw = (width_pixels + 63) / 64;
    if (psm & 2) {
        fh = (height_pixels + 63) / 64;
    } else {
        fh = (height_pixels + 31) / 32;
    }
    if ((*(u64 *)gp & 0x0000FFFF0000FFFFULL) == 1) {
        return fw * fh;
    }
    return fw * fh * 2;
}
