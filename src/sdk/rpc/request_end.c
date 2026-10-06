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
struct EndPkt {
    u8 pad_0[0x1C];
    struct sceSifClientData *cd;
    s32 cid;
    s32 sd;
    s32 buf;
    s32 cbuf;
};
struct sceSifClientData {
    s32 pkt_addr;
    u8 pad_4[4];
    s32 sema_id;
    u8 pad_C[8];
    s32 buf;
    s32 cbuf;
    s32 (*end_function)(s32);
    s32 end_param;
    s32 server;
};
extern s32 iSignalSema();
extern s32 func_0011AD90();
void _request_end(struct EndPkt *request, void *data) {
    struct sceSifClientData *cd;
    (void)data;
    if (request->cid == 0x8000000A)
        goto case_a;
    if ((u32)request->cid > 0x8000000AU)
        goto tail;
    if (request->cid == 0x80000009)
        goto case_b;
    goto tail;
case_a:
    cd = request->cd;
    if (cd->end_function != 0) {
        cd->end_function(cd->end_param);
    }
    goto tail;
case_b:
    cd = request->cd;
    cd->server = request->sd;
    cd->buf = request->buf;
    cd->cbuf = request->cbuf;
    goto tail;
tail:
    cd = request->cd;
    if (cd->sema_id >= 0) {
        iSignalSema(cd->sema_id);
    }
    func_0011AD90((u32 *)cd->pkt_addr);
    cd->pkt_addr = 0;
}
