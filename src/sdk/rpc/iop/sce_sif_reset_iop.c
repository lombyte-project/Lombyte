#include "types.h"
typedef struct {
    u64 psize : 8;
    u64 dsize : 24;
    u64 dest : 32;
    s32 cid;
    u32 opt;
} SifCmdHeader;

typedef struct {
    SifCmdHeader header;
    s32 arglen;
    s32 mode;
    char arg[80];
} SifResetPacket;

typedef struct {
    u32 data;
    u32 addr;
    s32 size;
    s32 mode;
} SifDmaData;

extern SifResetPacket D_00158440;
extern void sceSifStopDma(void);
extern u32 sceSifGetReg(u32 reg);
extern u32 sceSifSetReg(u32 reg, u32 val);
extern void sceSifWriteBackDCache(void *p, s32 size);
extern u32 sceSifSetDma(SifDmaData *dma, s32 count);

s32 sceSifResetIop(const char *arg, s32 mode) {
    SifDmaData dma;
    u32 addr;
    s32 arglen;

    sceSifStopDma();
    addr = sceSifGetReg(0x80000000);
    D_00158440.mode = mode;
    for (arglen = 0; arg[arglen] != 0; arglen++) {
        D_00158440.arg[arglen] = arg[arglen];
    }
    D_00158440.header.dest = 0;
    D_00158440.arglen = arglen;
    D_00158440.header.cid = 0x80000003;
    D_00158440.header.dsize = 0;
    D_00158440.header.psize = sizeof(SifResetPacket);
    dma.data = (u32)&D_00158440;
    dma.addr = addr;
    dma.size = sizeof(SifResetPacket);
    dma.mode = 0x44;
    sceSifWriteBackDCache(&D_00158440, sizeof(SifResetPacket));
    sceSifSetReg(4, 0x40000);
    if (sceSifSetDma(&dma, 1)) {
        sceSifSetReg(4, 0x10000);
        sceSifSetReg(4, 0x20000);
        sceSifSetReg(0x80000002, 0);
        sceSifSetReg(0x80000000, 0);
        return 1;
    }
    return 0;
}
