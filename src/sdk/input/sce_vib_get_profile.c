#include "types.h"

typedef struct {
    u64 size : 14;
    u64 mode : 2;
    u64 cmd  : 8;
    u64 ver  : 8;
    u64 pad  : 32;
} DbcCommand;

typedef union {
    DbcCommand bits;
    u64        word;
} DbcCommandWord;

extern s32 sceDbcReceiveData(s32 port, DbcCommand cmd, void *buf, void *data);

s32 sceVibGetProfile(s32 port, u8 *profile) {
    DbcCommand cmd;
    s32        result;
    s32        receive_status;

    cmd.size = 2;
    cmd.mode = 2;
    cmd.cmd = 3;
    cmd.ver = 1;
    receive_status = sceDbcReceiveData(port, cmd, &result, profile);
    if (receive_status < 0) {
        return receive_status;
    }
    return result;
}
