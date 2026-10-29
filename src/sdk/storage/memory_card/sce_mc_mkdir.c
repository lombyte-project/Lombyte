#include "sda.h"
typedef signed int s32;

extern s32 sceMcOpen(s32 port, s32 slot, const char *name, s32 flags);
extern s32 D_00132DA8 NOT_SDA;

s32 sceMcMkdir(s32 port, s32 slot, const char *name) {
    s32 result = sceMcOpen(port, slot, name, 0x40);

    if (result == 0) {
        D_00132DA8 = 0xB;
    }
    return result;
}
