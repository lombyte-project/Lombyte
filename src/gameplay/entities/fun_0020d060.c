#include "types.h"
struct DmaTag { u32 w0; u32 addr; u32 w2; u32 w3; };
struct TagPtr { struct DmaTag *p; };
extern struct TagPtr D_00160F00;
extern struct TagPtr D_0015FF40;
extern s32 D_0015FF3C;
extern u8 D_001C8680[];
extern u8 D_0015FEC0[];
extern void FUN_00227740(void);
extern void FUN_00227548(void *);
extern void func_001F21B0(void *, s32);
void FUN_0020d060(void) {
    struct DmaTag *tag;

    if (D_0015FF3C == 0) {
        D_0015FF40.p->w0 = 0x10000000;
        D_0015FF40.p->addr = 0;
        D_0015FF40.p->w2 = 0;
        D_0015FF40.p->w3 = 0;
        return;
    }
    tag = D_00160F00.p++;
    D_0015FF40.p->w0 = 0x20000000;
    D_0015FF40.p->addr = (u32)D_00160F00.p;
    D_0015FF40.p->w2 = 0;
    D_0015FF40.p->w3 = 0;
    FUN_00227740();
    FUN_00227548(D_001C8680);
    D_00160F00.p->w0 = 0x20000000;
    D_00160F00.p->addr = (u32)(D_0015FF40.p + 1);
    D_00160F00.p->w2 = 0;
    D_00160F00.p->w3 = 0;
    D_00160F00.p++;
    tag->w0 = 0x20000000;
    tag->addr = (u32)D_00160F00.p;
    tag->w2 = 0;
    tag->w3 = 0;
    func_001F21B0(D_0015FEC0, 8);
}

extern __typeof__(FUN_0020d060) func_0020D060 __attribute__((alias("FUN_0020d060")));
