/* topThread t1: kernel thread dispatcher loop. */

#include "types.h"
#include "rnc/sdk/top_thread.h"

extern u8 D_001526E8[];

extern s32 WaitSema(s32 sema);
extern s32 WakeupThread(s32 id);
extern s32 RotateThreadReadyQueue(s32 priority);
extern s32 SuspendThread(s32 id);
extern s32 kprintf(const char *fmt, ...);

void topThread(TopThreadQueue *queue) {
    s32 i;
    u8 type;

    for (;;) {
        WaitSema(top_thread_semaphore);
        i = queue->read_index & 0x1FF;
        queue->read_index = i + 1;
        type = queue->requests[i].command;
        switch (type) {
        case 0:
            WakeupThread(queue->requests[i].argument);
            break;
        case 1:
            RotateThreadReadyQueue(queue->requests[i].argument);
            break;
        case 2:
            SuspendThread(queue->requests[i].argument);
            break;
        default:
            kprintf((const char *)D_001526E8);
            break;
        }
    }
}
