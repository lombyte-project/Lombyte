#include "types.h"
#include "rnc/sdk/sifrpc.h"
#include "rnc/data_labels.h"
#include "rnc/sdk/rpc/cmd.h"
#include "rnc/sdk/rpc/rpc.h"

struct SifRpcPacket {
    u8  pad_0[0x14];
    s32 pkt_addr;
    s32 rpc_id;
    s32 client;
    s32 sid;
};

extern s32 CreateSema();
extern s32 DeleteSema();
extern s32 WaitSema();
extern s32 get_available_rpc_packet() __asm__("func_0011ACE8");

s32 sceSifBindRpc(struct sceSifClientData *client, s32 rpc_number, s32 mode) {
    s32                  sema_param[6];
    s32                  sema_id;
    struct SifRpcPacket *packet;

    client->command = 0;
    client->serve = 0;
    packet = (struct SifRpcPacket *)get_available_rpc_packet(D_00156800);
    if (packet == NULL) {
        return -1;
    }
    client->rpcd.pid = packet->rpc_id;
    client->rpcd.paddr = packet;
    packet->sid = rpc_number;
    packet->pkt_addr = (s32)packet;
    packet->client = (s32)client;
    if (mode & 1) {
        goto block_7;
    }
    sema_param[1] = 1;
    sema_param[2] = 0;
    sema_id = CreateSema(sema_param);
    client->rpcd.tid = sema_id;
    if (sema_id >= 0) {
        goto block_4;
    }
    free_rpc_packet(packet);
    return -3;
block_4:
    if (sceSifSendCmd(0x80000009, packet, 0x40, 0, 0, 0) != 0) {
        goto block_6;
    }
    free_rpc_packet(packet);
    DeleteSema(client->rpcd.tid);
    return -2;
block_6:
    WaitSema(client->rpcd.tid);
    DeleteSema(client->rpcd.tid);
    return 0;
block_7:
    client->rpcd.tid = -1;
    if (sceSifSendCmd(0x80000009, packet, 0x40, 0, 0, 0) != 0) {
        return 0;
    }
    free_rpc_packet(packet);
    return -2;
}
