/* InitThread i4: create the kernel patch sema and top-level thread. */

#include "types.h"
#include "kernel.h"
#include "rnc/sdk/top_thread.h"


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
extern void topThread(TopThreadQueue *queue);

extern s32 D_00154100[];
extern s32 D_00166C00[];

s32 InitThread(void) {
    struct ThreadParam th;
    struct SemaParam se;

    if (top_thread_id > 0)
        return -1;

    se.initCount = 0;
    se.maxCount = 0xFF;
    top_thread_semaphore = CreateSema(&se);
    if (top_thread_semaphore < 0)
        return -1;

    th.entry = topThread;
    th.stack = D_00154100;
    th.stackSize = 0x400;
    th.gp = D_00166C00;
    th.priority = 0;
    top_thread_id = CreateThread(&th);
    if (top_thread_id < 0) {
        DeleteSema(top_thread_semaphore);
        return -1;
    }

    top_thread_queue.read_index = 0;
    top_thread_queue.write_index = 0;
    _StartThread(top_thread_id, &top_thread_queue);
    ChangeThreadPriority(GetThreadId(), 1);

    return top_thread_id;
}
