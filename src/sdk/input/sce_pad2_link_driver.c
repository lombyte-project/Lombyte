#include "types.h"
struct Pad {
    u8 pad[4];
    s32 unk4;
    s32 unk8;
    u8 rest[0x324];
};
extern struct Pad D_0015B540[];
extern s32 sceDbcGetDepNumber();
s32 scePad2LinkDriver(s32 arg0) {
    s32 dep;
    dep = sceDbcGetDepNumber();
    if (dep < 0)
        return dep;
    D_0015B540[arg0].unk8 = dep;
    D_0015B540[arg0].unk4 = 1;
    return dep;
}
