/* Ported from rac1-decomp (src/core/00119D88.c: func_0011BCB0). */
#include "types.h"
#include "sda.h"

struct RpcCb { void (*cb)(void *); void *data; };
extern struct RpcCb D_00158000 NOT_SDA;
extern char D_00157FC0[];
extern char D_00157F80[];
extern char D_001574C0[];
extern char *D_00156840[];
extern char D_00157FA8[];
extern char D_00157D80[];
extern char D_00157500[];
extern int D_0012FC94 NOT_SDA;
extern int D_0012FC98[];
extern int D_0012FCA0 NOT_SDA;
extern void _sceFs_Rcv_Intr();
extern void invoke_sif_file_command_callback() __asm__("func_0011B980");
extern void sceSifInitRpc(u32 mode);
extern s32 DIntr();
extern s32 EnableInterrupts();
extern s32 SignalSema();
extern s32 WaitSema();
extern s32 _sceFsIobSemaMK();
extern s32 sceSifAddCmdHandler();
extern s32 sceSifBindRpc();
extern s32 sceSifCallRpc();

struct W4 { char b[4]; };
s32 sceFsInit(void) {
    s32 r, i;
    char *p;
    struct W4 w;

    struct RpcCb *rq = &D_00158000;
    sceSifInitRpc(0);
    D_00158000.cb = 0;
    rq->data = 0;
    r = DIntr();
    sceSifAddCmdHandler(0x80000011, (int)_sceFs_Rcv_Intr, (int)D_00157FC0);
    sceSifAddCmdHandler(0x80000013, (int)invoke_sif_file_command_callback, (int)rq);
    if (r != 0) {
        EnableInterrupts();
    }
    for (;;) {
        r = sceSifBindRpc(D_00157F80, 0x80000001, 0);
        if (r < 0) {
            return -1;
        }
        if (*(s32 *)(D_00157F80 + 0x24) != 0) {
            break;
        }
        for (i = 0x100000; i != -1; i--) {
        }
    }
    _sceFsIobSemaMK();
    WaitSema(D_0012FCA0);
    for (p = D_00157D80; p < D_00157D80 + 0x200; p += 0x10) {
        *(s32 *)(p + 4) = 0;
    }
    SignalSema(D_0012FCA0);
    D_00156840[0] = D_00157500;
    D_00156840[1] = D_00157500 + 0x440;
    r = sceSifCallRpc(D_00157F80, 0xFF, 0, D_00156840, 8, D_001574C0, 8, 0, 0);
    if (r < 0) {
        return (s32)0xFFFEFFFF;
    }
    *(struct W4 *)D_00157FA8 = *(struct W4 *)((u32)D_001574C0 | 0x20000000);
    w = *(struct W4 *)((u32)(D_001574C0 + 4) | 0x20000000);
    D_0012FC98[0] = (*(s32 *)&w == 2);
    D_0012FC94 = 1;
    return 0;
}
