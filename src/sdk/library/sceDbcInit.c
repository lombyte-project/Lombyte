#include "types.h"
typedef struct {
    u8 pad0[0x24];
    void *server;
} SifClientData;

extern SifClientData D_0015B008;
extern SifClientData D_0015B030;
extern u8 D_0015B480[];
extern s32 D_0015B500[];
extern char D_001534D8[];
extern char D_001534F0[];
extern char D_00153518[];
extern char D_00153540[];
extern void sceSifInitRpc(u32);
extern s32 sceSifBindRpc(SifClientData *, u32, u32);
extern void debug_print_stub(const char *fmt, ...) __asm__("func_00124A20");
extern void exit(s32);
extern s32 sceDbcGetModVersion(void);
extern void sceDbcSetWorkAddr(void *addr);
extern s32 scePrintf(const char *, ...);

s32 sceDbcInit(void) {
    s32 ver;
    s32 major;
    s32 i;

    sceSifInitRpc(0);
    while (1) {
        if (sceSifBindRpc(&D_0015B008, 0x80000900, 0) < 0) {
            debug_print_stub(D_001534D8);
            exit(-1);
        }
        if (D_0015B008.server != 0) {
            break;
        }
        i = 0x10000;
        do {
        } while (--i != -1);
    }
    while (1) {
        if (sceSifBindRpc(&D_0015B030, 0x8000091B, 0) < 0) {
            debug_print_stub(D_001534D8);
            exit(-1);
        }
        if (D_0015B030.server != 0) {
            break;
        }
        i = 0x10000;
        do {
        } while (--i != -1);
    }
    ver = sceDbcGetModVersion();
    major = ver >> 8;
    if (major != 2) {
        scePrintf(D_001534F0);
        scePrintf(D_00153518, 2, 0, major, ver & 0xFF);
        return 0;
    }
    for (i = 15; i >= 0; i--) {
        D_0015B500[i] = 0;
    }
    debug_print_stub(D_00153540, D_0015B480);
    sceDbcSetWorkAddr(D_0015B480);
    return 1;
}
