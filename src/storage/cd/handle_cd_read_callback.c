#include "types.h"
extern volatile s32 D_001312D4[];
extern volatile s32 D_001312E0[];
extern volatile s32 D_001312E8[];
extern volatile s32 D_001312F0[];
extern volatile s32 D_00131314[];
extern volatile s32 D_00131318[];
extern volatile s32 D_00159740[];
extern s32 iSignalSema();

void handle_cd_read_callback(s32 *status) __asm__("FUN_001206d8");

void handle_cd_read_callback(s32 *status) {
    D_00131314[0] = *status;
    D_00131318[0] = D_00131314[0];
    if (D_00131314[0] == 11) {
        D_00131314[0] = 0;
        D_001312F0[0] = 0;
        return;
    }
    iSignalSema(D_001312E8[0]);
    if (D_001312D4[0] != 0 && D_00159740[0] != 0) {
        iSignalSema(D_001312E0[0]);
    } else {
        D_001312F0[0] = 0;
    }
    D_00131314[0] = 0;
}
extern void func_001206D8(s32 *status) __attribute__((alias("FUN_001206d8")));
