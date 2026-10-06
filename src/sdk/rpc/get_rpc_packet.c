#include "types.h"
struct RpcPacketQueue {
    u8 pad_0[0x14];
    s32 unk14;
    s32 unk18;
    u8 pad_1C[0x8];
    s32 unk24;
};
s32 GetRpcPacket(struct RpcPacketQueue *queue) {
    struct RpcPacketQueue *alias;
    struct RpcPacketQueue *store;
    s32 remainder;
    s32 scaled;
    alias = queue;
    remainder = (s32)queue->unk24 % (s32)alias->unk18;
    scaled = remainder << 6;
    store = queue;
    store->unk24 = (s32)(remainder + 1);
    return store->unk14 + scaled;
}
