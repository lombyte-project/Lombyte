#include "types.h"

typedef struct {
    s32 option;
    s32 port;
    s32 slot;
    s32 number;
    u8  name[16];
} Pad2SocketParam;

typedef struct {
    s32 option;
    s32 type;
    s32 port;
    s32 slot;
    s32 number;
    u8  name[16];
} DbcSocketParam;

typedef struct {
    s32 active;
    u8  pad4[0x8];
    u8 *addr;
    u8  pad10[0x320];
} Pad2Info;

typedef struct {
    u8  f0;
    u8  f1;
    u8  f2;
    u8  f3;
    s32 f4;
    u8  pad8[0x14];
    u8  buttons[32];
    u8  pad3C[0x40];
    s32 f7C;
} Pad2Side;

extern Pad2Info D_0015B540[];
extern s32      sceDbcCreateSocket(DbcSocketParam *param, void *addr0, void *addr1);
extern void    *memset(void *, s32, u32);
extern void    *memcpy(void *, const void *, u32);

s32 scePad2CreateSocket(Pad2SocketParam *param, void *addr) {
    DbcSocketParam dbc;
    Pad2Side      *side;
    s32            socket;
    s32            i;

    if ((u32)addr & 0x3F) {
        return -1;
    }
    if (param != 0) {
        dbc.option = param->option;
        dbc.port = param->port;
        dbc.slot = param->slot;
        dbc.number = param->number;
        memcpy(dbc.name, param->name, 16);
    } else {
        dbc.option = 0;
        dbc.port = 0;
        dbc.slot = 0;
        dbc.number = 0;
        dbc.name[0] = 0;
    }
    dbc.type = 1;
    dbc.option |= 1;
    socket = sceDbcCreateSocket(&dbc, addr, (u8 *)addr + 128);
    if (socket < 0) {
        return socket;
    }
    D_0015B540[socket].active = 1;
    D_0015B540[socket].addr = addr;
    side = (Pad2Side *)addr;
    for (i = 0; i < 2; i++, side++) {
        side->f0 = 0;
        side->f7C = 0;
        side->f1 = 0;
        side->f3 = 0;
        side->f2 = 0;
        side->f4 = 0;
        memset(side->buttons, 0xFF, 32);
    }
    return socket;
}
