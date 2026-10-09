#include "types.h"
#include "rnc/sdk/sce_gs_gparam.h"

extern sceGsGParam *GetCoreDataTable(void);

s16 sceGszbufaddr(s16 psm, s16 w, s16 h) {
    sceGsGParam *gp;
    s32 fw;
    s32 fh;

    gp = GetCoreDataTable();
    fw = (w + 63) / 64;
    if (psm & 2) {
        fh = (h + 63) / 64;
    } else {
        fh = (h + 31) / 32;
    }
    if ((*(u64 *)gp & 0x0000FFFF0000FFFFULL) == 1) {
        return fw * fh;
    }
    return fw * fh * 2;
}
