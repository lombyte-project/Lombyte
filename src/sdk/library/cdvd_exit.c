#include "types.h"
extern s32 D_001312D4[]; extern s32 D_001312E0[]; extern s32 D_001312E8[]; extern s32 D_001312EC[]; extern s32 D_00131314[];
extern s32 DIntr(); extern s32 DeleteSema(); extern s32 SignalSema(); extern s32 sceSifRemoveCmdHandler(); extern s32 EnableInterrupts();
void cdvd_exit(void) {
    s32 temp_16_31;
    if (D_001312D4[0] != 0) {
        do {
            D_00131314[0] = -1;
        } while (0);
        SignalSema(D_001312E0[0]);
    }
    DeleteSema(D_001312E8[0]);
    DeleteSema(D_001312EC[0]);
    DeleteSema(D_001312E0[0]);
    temp_16_31 = DIntr();
    sceSifRemoveCmdHandler(0x80000012);
    if (temp_16_31 != 0) EnableInterrupts();
}
