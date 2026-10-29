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

typedef struct {
    u8  pad0[0x4];
    s32 linked;
    u8  pad8[0x328];
} Pad2Info;

typedef struct {
    u8  state;
    u8  pad1[0x3];
    s32 linked;
} Pad2Side;

extern Pad2Info  D_0015B540[];
extern s32       sceDbcReceiveData(s32 port, s32 cmd, void *buf, void *data);
extern s32       scePad2LinkDriver(s32 socket);
extern s32       scePad2CheckDma(s32 socket);
extern Pad2Side *scePad2GetSide(s32 socket);

s32 scePad2GetState(s32 socket) {
    DbcCommandWord cmd;
    Pad2Side      *side;
    u8             pad_state_buffer[16];

    if (D_0015B540[socket].linked == 0) {
        if (scePad2LinkDriver(socket) < 0) {
            return 0;
        }
    }
    if (scePad2CheckDma(socket)) {
        side = scePad2GetSide(socket);
        if (side->linked == 0) {
            if (scePad2LinkDriver(socket) < 0) {
                D_0015B540[socket].linked = 0;
                return 0;
            }
        }
        pad_state_buffer[0] = side->state;
    } else {
        cmd.bits.size = 12;
        cmd.bits.mode = 2;
        cmd.bits.cmd = 1;
        cmd.bits.ver = 1;
        if (sceDbcReceiveData(socket, cmd.word, &pad_state_buffer[4], pad_state_buffer) < 0) {
            return 0;
        }
    }
    return pad_state_buffer[0];
}
