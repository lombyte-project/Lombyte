#include "types.h"
#include "rnc/sdk/sce_gs_gparam.h"
#include "rnc/sdk/graph/sce_gs_get_gparam.h"
#include "rnc/sdk/kernel.h"

s32 sceGsSyncV(s32 unused_mode) {
    sceGsGParam *gp;
    s64          field;

    gp = sceGsGetGParam();
    if (gp->sceGsVSCfunc == 0) {
        VSync();
        if (gp->sceGsInterMode == 1) {
            return (*(volatile u64 *)0x12001000 >> 13) & 1;
        }
    } else {
        field = (VSync2() >> 13) & 1;
        if (gp->sceGsInterMode == 1) {
            return field;
        }
    }
    return 1;
}
