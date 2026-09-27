#include "types.h"
struct SndCb { s32 func; s32 pad; s64 data; };
extern s32 D_0015ECC4 __attribute__((sda));
extern s32 D_0015EC80 __attribute__((sda));
extern s32 D_0015ECC0 __attribute__((sda));
extern s32 *D_0015ECA0[2] __attribute__((sda));
extern s32 D_0015ECA8[2] __attribute__((sda));
extern struct SndCb *D_0015ECB0[2] __attribute__((sda));
extern s32 D_00133100[4];
extern u8 D_0015EBC0[];
extern char D_00153D20[];
extern char D_00154010[];
extern char D_00154070[];
extern s32 printf(const char *, ...);
extern s32 func_0012DC80(void);
extern void func_0012E9A0(void);
extern s32 FlushCache(s32);
extern s32 StoreObjectIndex(void *, s32);
extern s32 SceSifCheckStatRpc(void *);
extern s32 sceSifCallRpc(void *, s32, s32, void *, s32, void *, s32, void *, void *);
void snd_send_iop_command_no_wait(s32 cmd, s32 size, u8 *data, s32 cb, s64 cb_data) __asm__("FUN_0012e6e0");

void snd_send_iop_command_no_wait(s32 cmd, s32 size, u8 *data, s32 cb, s64 cb_data) {
    s32 need;
    s32 tries;
    s32 held;
    s32 i;
    u8 *p;

    held = 0;
    if (D_0015ECC4 == 0 && D_0015EC80 == 0 && size == 0 && cb == 0) {
        StoreObjectIndex(D_00133100, 1);
        while (SceSifCheckStatRpc(D_0015EBC0) != 0) {
            printf(D_00153D20);
            func_0012DC80();
            FlushCache(0);
        }
        sceSifCallRpc(D_0015EBC0, cmd, 1, 0, 0, D_00133100, 0xC, 0, 0);
        return;
    }
    need = size + 4;
    tries = 0;
    if (need % 4 != 0) {
        need += 4 - need % 4;
    }
    while (*D_0015ECA0[D_0015ECC0] == 0x100 || D_0015ECA8[D_0015ECC0] < need) {
        if (D_0015ECC4 != 0) {
            D_0015ECC4 = 0;
            held = 1;
        }
        func_0012DC80();
        if (tries == 1) {
            printf(D_00154010, D_0015ECC0, *D_0015ECA0[D_0015ECC0]);
        }
        tries++;
    }
    if (tries != 0) {
        printf(D_00154070, tries);
    }
    if (held != 0) {
        D_0015ECC4 = 1;
    }
    p = (u8 *)D_0015ECA0[D_0015ECC0] - (D_0015ECA8[D_0015ECC0] - 0x1000);
    *(s16 *)p = cmd;
    p += 2;
    *(s16 *)p = size;
    p += 2;
    for (i = 0; i < size; i++) {
        p[i] = data[i];
    }
    D_0015ECA8[D_0015ECC0] -= need;
    D_0015ECB0[D_0015ECC0][*D_0015ECA0[D_0015ECC0]].func = cb;
    D_0015ECB0[D_0015ECC0][*D_0015ECA0[D_0015ECC0]].data = cb_data;
    func_0012E9A0();
}

extern __typeof__(snd_send_iop_command_no_wait) func_0012E6E0 __attribute__((alias("FUN_0012e6e0")));
