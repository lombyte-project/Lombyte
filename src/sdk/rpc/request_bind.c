typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
struct RpcBindRequest {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x4];
    s32 unk1C;
    s32 unk20;
};
struct RpcBindPacket {
    u8 pad_0[0x14];
    s32 unk14;
    u8 pad_18[0x4];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
};
struct sceSifServeData {
    u8 pad_0[0x8];
    s32 unk8;
    u8 pad_C[0x8];
    s32 unk14;
};
extern s32 GetRpcPacket();
extern s32 SearchSvdata();
extern void isceSifSendCmd();
void _request_bind(struct RpcBindRequest *arg0, s32 arg1) {
    struct RpcBindPacket *packet;
    s32 new_var;
    struct sceSifServeData *svdata;
    packet = GetRpcPacket(arg1);
    new_var = (s32)arg0->unk14;
    packet->unk1C = (s32)arg0->unk1C;
    packet->unk14 = new_var;
    packet->unk20 = 0x80000009;
    svdata = SearchSvdata(arg0->unk20, arg1);
    if (svdata == 0) {
        packet->unk24 = 0;
        packet->unk28 = 0;
        packet->unk2C = 0;
    } else {
        packet->unk24 = svdata;
        packet->unk28 = (s32)svdata->unk8;
        packet->unk2C = (s32)svdata->unk14;
    }
    isceSifSendCmd(0x80000008, packet, 0x40, 0, 0, 0);
}
