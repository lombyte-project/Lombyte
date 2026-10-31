#include "types.h"
#include "rnc/sdk/libcdvd.h"
#include "rnc/sdk/interrupts/enable_interrupts.h"
#include "rnc/sdk/rpc/cmd/sce_sif_remove_cmd_handler.h"

extern s32 D_001312D4[];
extern s32 D_00131314[];
extern s32 DIntr();
extern s32 DeleteSema();
extern s32 SignalSema();

void cdvd_exit(void) {
    s32 interrupts_enabled;
    if (D_001312D4[0] != 0) {
        do {
            D_00131314[0] = -1;
        } while (0);
        SignalSema(cd_callback_sema);
    }
    DeleteSema(cd_ncmd_sema);
    DeleteSema(cd_scmd_sema);
    DeleteSema(cd_callback_sema);
    interrupts_enabled = DIntr();
    sceSifRemoveCmdHandler(0x80000012);
    if (interrupts_enabled != 0)
        EnableInterrupts();
}
