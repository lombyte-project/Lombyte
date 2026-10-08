#include "types.h"
#include "sifrpc.h"
struct SifRpcPacket {
    u8 pad_0[0x14];
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
};

extern u8 D_00156800[];
extern s32 CreateSema();
extern s32 DeleteSema();
extern s32 WaitSema();
extern s32 get_available_rpc_packet() __asm__("func_0011ACE8");
extern s32 func_0011AD90();
extern s32 sceSifSendCmd();
s32 sceSifBindRpc(struct sceSifClientData *client, s32 rpc_number, s32 mode) {
    s32 sema_param[6];
    s32 sema_id;
    struct SifRpcPacket *packet;

    client->command = 0;
    client->serve = 0;
    packet = (struct SifRpcPacket *)get_available_rpc_packet(D_00156800);
    if (packet == NULL) {
        return -1;
    }
    client->rpcd.pid = packet->unk18;
    client->rpcd.paddr = packet;
    packet->unk20 = rpc_number;
    packet->unk14 = (s32)packet;
    packet->unk1C = (s32)client;
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
    func_0011AD90(packet);
    return -3;
block_4:
    if (sceSifSendCmd(0x80000009, packet, 0x40, 0, 0, 0) != 0) {
        goto block_6;
    }
    func_0011AD90(packet);
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
    func_0011AD90(packet);
    return -2;
}
