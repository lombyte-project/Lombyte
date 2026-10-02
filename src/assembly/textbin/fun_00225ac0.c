#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_00225ac0/FUN_00225ac0.s", FUN_00225ac0);
#else
#include "types.h"

struct FrameBuf { s32 addr; s32 flag; };
struct GameState { u8 pad0[0x108]; s32 buf_b; s32 buf_a; };

extern struct GameState D_001D5BF0;
extern struct FrameBuf D_001D60B8[5];

void FUN_00225ac0(s32 mode) {
    s32 a;
    s32 b;
    s32 n1;
    s32 n2;
    s32 n3;
    s32 i;
    s32 end;
    struct FrameBuf *p;

    b = D_001D5BF0.buf_b;
    a = D_001D5BF0.buf_a;
    if (mode == 0) {
        n1 = 1;
        n2 = 0;
        n3 = 0;
    } else {
        n1 = 2;
        n2 = 1;
        n3 = 2;
    }
    end = n1;
    i = 0;
    if (n1 > 0) {
        p = D_001D60B8;
        for (i = n1; i != 0; i--) {
            p->addr = b;
            p->flag = 0;
            b += 0x11800;
            p++;
        }
        i = n1;
    }
    end += n2;
    for (; i < end; i++) {
        D_001D60B8[i].addr = a;
        D_001D60B8[i].flag = 0;
        a += 0x11800;
    }
    end += n3;
    for (; i < end; i++) {
        D_001D60B8[i].addr = b;
        D_001D60B8[i].flag = 1;
        b += 0x4F000;
    }
    for (; i < 5; i++) {
        D_001D60B8[i].flag = 0;
        D_001D60B8[i].addr = 0;
    }
}
#endif /* NON_MATCHING */
