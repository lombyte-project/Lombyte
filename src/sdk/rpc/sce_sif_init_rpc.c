#include "types.h"
#include "rnc/runtime/core_state.h"
typedef struct {
    s32 pid;
    void *pkt_table;
    s32 pkt_table_len;
    s32 unk0C;
    s32 unk10;
    void *rdata_table;
    s32 rdata_table_len;
    void *client_table;
    s32 client_table_len;
    s32 rdata_table_idx;
} SifRpcData;

extern u8 D_00155000[];
extern u8 D_00155800[];
extern u8 D_00156000[];
extern SifRpcData D_00156800;
extern void _request_end();
extern void _request_bind();
extern void _request_rdata();
extern void D_0011B138();
extern s32 DIntr(void);
extern s32 EnableInterrupts(void);
extern void sceSifInitCmd(void);
extern void sceSifAddCmdHandler(u32 cid, void *handler, void *data);
extern u32 sceSifGetReg(u32 reg);
extern u32 sceSifSetReg(u32 reg, u32 val);
extern u32 sceSifSendCmd(u32 cid, void *packet, s32 size, void *src, void *dst, s32 dsize);
extern u32 func_0011A458(u32 reg);

void sceSifInitRpc(u32 mode) {
    s32 *packet;

    DIntr();
    if (RpcCommandState) {
        EnableInterrupts();
        return;
    }
    RpcCommandState = 1;
    EnableInterrupts();
    sceSifInitCmd();
    DIntr();
    D_00156800.pkt_table = (void *)((u32)D_00155000 | 0x20000000);
    D_00156800.pkt_table_len = 32;
    D_00156800.unk0C = 0;
    D_00156800.unk10 = 0;
    D_00156800.rdata_table = (void *)((u32)D_00155800 | 0x20000000);
    D_00156800.rdata_table_len = 32;
    D_00156800.client_table = (void *)((u32)D_00156000 | 0x20000000);
    D_00156800.client_table_len = 32;
    D_00156800.rdata_table_idx = 0;
    D_00156800.pid = 1;
    sceSifAddCmdHandler(0x80000008, _request_end, &D_00156800);
    sceSifAddCmdHandler(0x80000009, _request_bind, &D_00156800);
    sceSifAddCmdHandler(0x8000000A, D_0011B138, &D_00156800);
    sceSifAddCmdHandler(0x8000000C, _request_rdata, &D_00156800);
    EnableInterrupts();
    if (sceSifGetReg(0x80000002)) {
        return;
    }
    packet = (s32 *)(D_00155000 + 64);
    packet[3] = 1;
    sceSifSendCmd(0x80000002, packet, 16, 0, 0, 0);
    while (func_0011A458(0) == 0) {
    }
    sceSifSetReg(0x80000002, 1);
}
