#include "types.h"
#include "rnc/sdk/sce_gs_gparam.h"
#include "rnc/sdk/graph/sce_gs_get_gparam.h"

extern s32  GsPutIMR();
extern s32  RemoveIntcHandler();
extern s32  disable_intc() __asm__("func_00119028");
extern void SetGsCrt(s32, s32, s32);

void sceGsResetGraph(s16 mode, s16 inter, s16 out, s16 ff) {
    enum {
        GS_RESET_AND_CONFIGURE_VIDEO = 0,
        GS_UPDATE_VIDEO_MODE = 5
    };
    sceGsGParam *gp;

    switch (mode) {
    case GS_RESET_AND_CONFIGURE_VIDEO:
        gp = sceGsGetGParam();
        *(volatile u64 *)0x12001000 = 0x200;
        gp->sceGsInterMode = inter;
        gp->sceGsOutMode = out;
        gp->sceGsVersion = (s16)((*(volatile u64 *)0x12001000 >> 16) & 0xFF);
        GsPutIMR(0xFF00);
        gp->sceGsFFMode = (ff != 0);
        if (gp->sceGsVSCfunc != 0) {
            disable_intc(2);
            RemoveIntcHandler(2, gp->sceGsVSCid);
            gp->sceGsVSCfunc = 0;
            gp->sceGsVSCid = 0;
        }
        SetGsCrt(inter & 1, out & 0xFF, ff & 1);
        break;
    case 1:
        *(volatile u64 *)0x12001000 = 0x100;
        break;
    case GS_UPDATE_VIDEO_MODE:
        gp = sceGsGetGParam();
        gp->sceGsFFMode = (ff != 0);
        gp->sceGsInterMode = inter;
        gp->sceGsOutMode = out;
        gp->sceGsVersion = (s16)((*(volatile u64 *)0x12001000 >> 16) & 0xFF);
        SetGsCrt(inter & 1, out & 0xFF, ff & 1);
        break;
    }
}
