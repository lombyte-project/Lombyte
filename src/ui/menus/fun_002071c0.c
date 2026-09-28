#include "types.h"

extern s32 func_00208818();
extern s32 D_001A03B4[];

s32 FUN_002071c0(s32 arg0, s32 arg1) {
    s32 ret1;
    s32 ret2;

    ret1 = func_00208818(arg0, arg1, 0x99, 0xED, 0x160, 0x117);
    ret2 = func_00208818(arg0, arg1, 0x10E, 0xF7, 0x13D, 0x119);
    if (D_001A03B4[0] == 0) {
        return 0;
    }
    if (ret1 == 0) {
        if (ret2 != 0) {
            if (ret1) {
                return 1;
            } else {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}
