#include "types.h"

extern s32 D_0015ED80[];
extern s32 D_0015EE78[];
extern s32 D_0015EE78_gp;
extern s32 D_0015F618[];
extern s32 D_00160EE0[];
extern s32 D_00160F00;
extern s32 func_001204B8();
extern s32 func_001F34E8();
extern s32 func_0020B418();
extern s32 func_002335D0();
extern s32 func_00233D00();
extern s32 func_00233D90();
extern s32 sceDmaReset();
extern s32 sceGsResetGraph();

void reset_graphics(void) __asm__("FUN_001f21c0");

void reset_graphics(void) {
    s32 temp_16_31;

    /* Schedule-critical shapes: the dead `temp_16_31 = 0` in the fourth
       argument and the single-iteration do-while reproduce retail's register
       allocation and the D_0015F618/D_00160EE0 $at store order. */
    D_0015F618[0] = 1;
    D_00160EE0[0] = 0;
    func_00233D90();
    sceDmaReset(1);
    func_0020B418();
    sceGsResetGraph(0, 1, (D_0015ED80[0] != 0) ? 3 : 2, temp_16_31 = 0);
    func_001204B8();
    do {
        func_002335D0();
        temp_16_31 = D_0015EE78[0];
    } while (0);
    D_00160F00 = 0;
    func_001F34E8();
    D_0015EE78_gp = temp_16_31;
    func_002335D0();
    func_00233D00();
}
