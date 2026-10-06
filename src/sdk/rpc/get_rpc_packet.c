#include "types.h"
struct M2c_arg0 {
    u8 pad_0[0x14];
    s32 unk14;
    s32 unk18;
    u8 pad_1C[0x8];
    s32 unk24;
};
s32 GetRpcPacket(struct M2c_arg0 *queue) {
    struct M2c_arg0 *alias;
    struct M2c_arg0 *store;
    s32 remainder;
    s32 scaled;
    alias = queue;
    remainder = (s32)queue->unk24 % (s32)alias->unk18;
    scaled = remainder << 6;
    store = queue;
    store->unk24 = (s32)(remainder + 1);
    return store->unk14 + scaled;
}
