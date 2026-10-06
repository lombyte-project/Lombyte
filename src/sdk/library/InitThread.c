/* InitThread i4: create the kernel patch sema and top-level thread. */

#include "types.h"

struct SemaParam {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[0x14];
};

struct ThreadParam {
    u8 pad0[4];
    void (*entry)();
    void *stack;
    s32 stackSize;
    void *gp;
    s32 priority;
    u8 pad18[0x18];
};

extern s32 CreateSema();
extern s32 CreateThread();
extern s32 DeleteSema(s32 sema);
extern s32 _StartThread(s32 id, void *arg);
extern s32 GetThreadId();
extern s32 ChangeThreadPriority(s32 id, s32 priority);
extern void topThread();

extern s32 D_0012FBF8[];
extern s32 D_00154500[];
extern u32 D_00154508[];
extern s32 D_00154100[];
extern s32 D_00166C00[];

s32 InitThread(void) {
    struct ThreadParam th;
    struct SemaParam se;

    if (D_0012FBF8[0] > 0)
        return -1;

    se.unk8 = 0;
    se.unk4 = 0xFF;
    D_00154500[0] = CreateSema(&se);
    if (D_00154500[0] < 0)
        return -1;

    th.entry = topThread;
    th.stack = D_00154100;
    th.stackSize = 0x400;
    th.gp = D_00166C00;
    th.priority = 0;
    D_0012FBF8[0] = CreateThread(&th);
    if (D_0012FBF8[0] < 0) {
        DeleteSema(D_00154500[0]);
        return -1;
    }

    D_00154508[0] = 0;
    D_00154508[1] = 0;
    _StartThread(D_0012FBF8[0], D_00154508);
    ChangeThreadPriority(GetThreadId(), 1);

    return D_0012FBF8[0];
}
