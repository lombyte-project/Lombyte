#include "types.h"

struct Pad {
    u8  pad[4];
    s32 linked;
    s32 dep_number;
    u8  rest[0x324];
};

extern struct Pad D_0015B540[];
extern s32        sceDbcGetDepNumber();

s32 scePad2LinkDriver(s32 socket_index) {
    s32 dep;
    dep = sceDbcGetDepNumber();
    if (dep < 0)
        return dep;
    D_0015B540[socket_index].dep_number = dep;
    D_0015B540[socket_index].linked = 1;
    return dep;
}
