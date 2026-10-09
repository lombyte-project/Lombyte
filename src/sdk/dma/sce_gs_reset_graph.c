#include "types.h"
#include "rnc/sdk/sce_gs_gparam.h"

extern sceGsGParam *GetCoreDataTable(void);
extern s32 GsPutIMR();
extern s32 RemoveIntcHandler();
extern s32 disable_intc() __asm__("func_00119028");
extern void SetGsCrt(s32, s32, s32);

void sceGsResetGraph(s16 mode, s16 inter, s16 out, s16 ff) {
    sceGsGParam *gp;

    switch (mode) {
    case 0:
        gp = GetCoreDataTable();
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
    case 5:
        gp = GetCoreDataTable();
        gp->sceGsFFMode = (ff != 0);
        gp->sceGsInterMode = inter;
        gp->sceGsOutMode = out;
        gp->sceGsVersion = (s16)((*(volatile u64 *)0x12001000 >> 16) & 0xFF);
        SetGsCrt(inter & 1, out & 0xFF, ff & 1);
        break;
    }
}
