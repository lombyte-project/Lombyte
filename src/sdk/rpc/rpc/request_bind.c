typedef signed char        s8;
typedef unsigned char      u8;
typedef signed short       s16;
typedef unsigned short     u16;
typedef signed int         s32;
typedef unsigned int       u32;
typedef signed long long   s64;
typedef unsigned long long u64;
typedef float              f32;
typedef double             f64;

struct RpcBindRequest {
    u8  pad_0[0x14];
    s32 pkt_addr;
    u8  pad_18[0x4];
    s32 client;
    s32 sid;
};

struct RpcBindPacket {
    u8  pad_0[0x14];
    s32 pkt_addr;
    u8  pad_18[0x4];
    s32 client;
    s32 cid;
    s32 server;
    s32 buff;
    s32 cbuff;
};

struct sceSifServeData {
    u8  pad_0[0x8];
    s32 buff;
    u8  pad_C[0x8];
    s32 cbuff;
};

extern s32  GetRpcPacket();
extern s32  SearchSvdata();
extern void isceSifSendCmd();

void _request_bind(struct RpcBindRequest *bind_request, s32 rpc_state) {
    struct RpcBindPacket   *packet;
    s32                     new_var;
    struct sceSifServeData *svdata;
    packet = GetRpcPacket(rpc_state);
    new_var = (s32)bind_request->pkt_addr;
    packet->client = (s32)bind_request->client;
    packet->pkt_addr = new_var;
    packet->cid = 0x80000009;
    svdata = SearchSvdata(bind_request->sid, rpc_state);
    if (svdata == 0) {
        packet->server = 0;
        packet->buff = 0;
        packet->cbuff = 0;
    } else {
        packet->server = svdata;
        packet->buff = (s32)svdata->buff;
        packet->cbuff = (s32)svdata->cbuff;
    }
    isceSifSendCmd(0x80000008, packet, 0x40, 0, 0, 0);
}
