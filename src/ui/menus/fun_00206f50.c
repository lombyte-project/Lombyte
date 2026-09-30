#include "types.h"
extern s32 func_00208818();

s32 FUN_00206f50(s32 arg0, s32 arg1) {
    if (arg1 < 0xE9) {
        if (func_00208818(arg0, arg1, 0x132, 0xA0, 0x15F, 0xD8)
            && func_00208818(arg0, arg1, 0x14D, 0xD8, 0x181, 0x9A)
            && func_00208818(arg0, arg1, 0x182, 0xB4, 0x137, 0x93)
            && func_00208818(arg0, arg1, 0x157, 0x8C, 0x130, 0xAA)) {
            return 1;
        }
        return 0;
    }
    {
        s32 a = func_00208818(arg0, arg1, 0x8F, 0x115, 0x148, 0x14B);
        s32 b = func_00208818(arg0, arg1, 0xE7, 0x108, 0x127, 0x164);
        if (a == 0 && b == 0) {
            return 0;
        }
    }
    {
        s32 a = func_00208818(arg0, arg1, 0xED, 0x15F, 0x154, 0x10E);
        s32 b = func_00208818(arg0, arg1, 0xA2, 0x12B, 0x16F, 0x147);
        if (a == 0 && b == 0) {
            return 0;
        }
    }
    {
        s32 a = func_00208818(arg0, arg1, 0x132, 0x163, 0x141, 0xCC);
        s32 b = func_00208818(arg0, arg1, 0xC2, 0x108, 0x1A0, 0x12E);
        if (!a && !b) {
            return 0;
        }
        return 1;
    }
}
