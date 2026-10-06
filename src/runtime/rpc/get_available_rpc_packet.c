#include "types.h"
typedef struct {
    s32 pid;
    void *pkt_table;
    s32 pkt_table_len;
} RpcData;

typedef struct {
    u8 pad0[0x10];
    s32 rec_id;
    void *pkt_addr;
    s32 rpc_id;
    u8 pad1C[0x24];
} RpcPacket;

extern s32 DIntr(void);
extern s32 EnableInterrupts(void);

void *get_available_rpc_packet(RpcData *rpc_data) __asm__("FUN_0011ace8");

void *get_available_rpc_packet(RpcData *rpc_data) {
    RpcPacket *packet;
    s32 len;
    s32 index;
    s32 rid;

    DIntr();
    len = rpc_data->pkt_table_len;
    packet = (RpcPacket *)rpc_data->pkt_table;
    for (index = 0; index < len; index++, packet++) {
        if (!(packet->rec_id & 1)) {
            packet->rec_id = (index << 16) | 5;
            if (++rpc_data->pid == 1) {
                rpc_data->pid++;
                rid = 1;
            } else {
                rid = rpc_data->pid;
            }
            packet->pkt_addr = packet;
            packet->rpc_id = rid;
            EnableInterrupts();
            return packet;
        }
    }
    EnableInterrupts();
    return 0;
}

extern void *func_0011ACE8(RpcData *rpc_data) __attribute__((alias("FUN_0011ace8")));
