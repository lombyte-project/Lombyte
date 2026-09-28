#include "types.h"
struct MenuItem { u8 pad0[0xA]; s16 state; };
struct Menu { u8 pad0[0x34]; struct MenuItem items[4]; };
struct Labels { u8 pad0[0x14]; s32 text[4]; };
extern struct Menu D_001D2800;
extern struct Labels D_001D2878;
extern s32 func_00215348(void);
extern s32 func_00215300(void);
void FUN_00226e58(void) {
    s32 a;
    s32 b;
    s32 c;

    a = func_00215348() >= 15;
    b = func_00215348() >= 30;
    c = func_00215300() >= 10;
    D_001D2800.items[0].state = a ? 3 : 2;
    D_001D2800.items[1].state = b ? 3 : 2;
    D_001D2800.items[2].state = c ? 10 : 2;
    D_001D2800.items[3].state = c ? 3 : 2;
    D_001D2878.text[0] = a ? 0x4FD3 : 0x4FD9;
    D_001D2878.text[1] = b ? 0x4FD4 : 0x4FDA;
    D_001D2878.text[2] = c ? 0x4FD7 : 0x4FDB;
    D_001D2878.text[3] = c ? 0x4FD8 : 0x4FDB;
}

extern __typeof__(FUN_00226e58) func_00226E58 __attribute__((alias("FUN_00226e58")));
