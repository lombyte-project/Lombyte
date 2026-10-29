#include "types.h"

typedef struct {
    u8 padding_00[0x10];
    s32 receive_id;
    u8 padding_14[0x4];
    s32 rpc_id;
} SifRpcPacket;

typedef struct {
    SifRpcPacket *packet;
    s32 rpc_id;
} SifRpcClient;

s32 SceSifCheckStatRpc(SifRpcClient *client) {
    SifRpcPacket *packet = client->packet;

    if ((packet == NULL) || (client->rpc_id != packet->rpc_id) || !(packet->receive_id & 1)) {
        return 0;
    }
    return 1;
}
