#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/textbin/fun_0021a1e0/FUN_0021a1e0.s", FUN_0021a1e0);
#else
#include "types.h"

struct Ids {
    s32 v[8];
};

struct Texts {
    s32 v[7];
};

struct MenuEntry {
    s32 text;
    u8 *flag;
    s32 unk8;
    s32 unkC;
    s32 unk10;
};

extern struct Ids D_001E8728;
extern struct Texts D_001E8748;
extern u8 D_0015EDC0[];
extern u8 D_0015EDB0[];
extern struct MenuEntry D_001D3B10[];

s32 FUN_0021a1e0(void) {
    struct Ids ids;
    struct Texts texts;
    struct MenuEntry *e;
    int nv;
    s32 n;
    s32 i;
    s32 id;

    nv = 0;
    ids = D_001E8728;
    texts = D_001E8748;
    do {
    n = 0;
    do {
    for (i = 0; i < 12; i++) {
        id = ids.v[i];
        if (id != -1 && D_0015EDC0[id] != 0) {
            D_001D3B10[n].flag = &D_0015EDB0[id];
            D_001D3B10[n].text = ids.v[i + 8];
            D_001D3B10[n].unk8 = 0x4F5A;
            D_001D3B10[n].unkC = 0x4F5B;
            D_001D3B10[n].unk10 = 0;
            n++;
        }
    }
    } while (0);
    } while (nv);
    D_001D3B10[n].text = 0;
    return 0;
}
#endif /* NON_MATCHING */
