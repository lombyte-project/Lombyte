#include "types.h"

struct Request {
    s32 state;
    s32 args[13];
};

extern struct Request D_001D5E88[];
extern s32 D_00160350;

s32 FUN_00226670(void) {
    s32 p;
    s32 end;
    s32 base;

    base = (s32)&D_001D5E88[0];
    p = base + 0x38;
    end = base + 0x1C0;
    do {
        *(struct Request *)p = *(struct Request *)(p + 0x38);
        p += 0x38;
    } while (p < end);
    D_00160350 = *(s32 *)0x00160350 - 1;
    return 0;
}

extern __typeof__(FUN_00226670) func_00226670 __attribute__((alias("FUN_00226670")));
