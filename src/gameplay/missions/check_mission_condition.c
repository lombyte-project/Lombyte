#include "types.h"
struct Upgrade {
    u8 pad0[0xC];
    s32 level;
};
extern u8 D_0013DD40[];
extern u8 D_0013D4C0[];
extern u8 D_0013D4E8[];
extern struct Upgrade D_0013D5B0[];
extern u8 D_0013D388[];
extern u8 D_0014BEC0[];
s32 check_mission_condition(s16 kind, s32 value) __asm__("FUN_0020baf0");

s32 check_mission_condition(s16 kind, s32 value) {
    switch (kind) {
    case 0:
        return 1;
    case 1:
        return D_0013DD40[value] != 0;
    case 2:
        return D_0013D4C0[value] != 0;
    case 3:
        return D_0013D4E8[value] != 0;
    case 4:
        if (value < 121) {
            return D_0013D5B0[value].level != 0;
        }
        break;
    case 5:
        if (value < 121) {
            return D_0013D5B0[value].level >= 2;
        }
        break;
    case 6:
        return D_0013D388[value] != 0;
    case 7:
        return ((s32(*)(void))value)() != 0;
    case 8:
        return D_0014BEC0[(value & 0xFFFF) + (value >> 16) * 4] != 0;
    case 9:
        return 0;
    }
    return 0;
}

extern __typeof__(check_mission_condition) func_0020BAF0 __attribute__((alias("FUN_0020baf0")));
