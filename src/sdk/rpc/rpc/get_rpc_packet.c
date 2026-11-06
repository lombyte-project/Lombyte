#include "types.h"

struct RpcPacketQueue {
    u8  pad_0[0x14];
    s32 rdata_table;
    s32 rdata_table_len;
    u8  pad_1C[0x8];
    s32 rdata_table_idx;
};

s32 GetRpcPacket(struct RpcPacketQueue *queue) {
    struct RpcPacketQueue *alias;
    struct RpcPacketQueue *store;
    s32                    remainder;
    s32                    scaled;
    alias = queue;
    remainder = (s32)queue->rdata_table_idx % (s32)alias->rdata_table_len;
    scaled = remainder << 6;
    store = queue;
    store->rdata_table_idx = (s32)(remainder + 1);
    return store->rdata_table + scaled;
}
