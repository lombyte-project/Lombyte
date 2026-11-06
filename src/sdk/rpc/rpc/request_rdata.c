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

struct RpcRdataRequest {
    u8  pad_0[0x14];
    s32 pkt_addr;
    u8  pad_18[0x4];
    s32 client;
    s32 src;
    s32 dest;
    s32 size;
};

struct RpcRdataPacket {
    u8  pad_0[0x14];
    s32 pkt_addr;
    u8  pad_18[0x4];
    s32 client;
    s32 cid;
};

extern s32  GetRpcPacket();
extern void isceSifSendCmd();

void _request_rdata(struct RpcRdataRequest *request, s32 rpc_state) {
    s32                    packet_address;
    s32                    client_address;
    struct RpcRdataPacket *packet;
    packet = GetRpcPacket(rpc_state);
    packet_address = (s32)request->pkt_addr;
    client_address = (s32)request->client;
    packet->pkt_addr = packet_address;
    packet->client = client_address;
    packet->cid = 0x8000000C;
    isceSifSendCmd(0x80000008, packet, 0x40, request->src, request->dest, request->size);
}
