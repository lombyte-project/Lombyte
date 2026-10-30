/* topThread t1: kernel thread dispatcher loop. */

#include "types.h"
#include "rnc/sdk/top_thread.h"

extern u8 thread_command_error_message[] __asm__("D_001526E8");

extern s32 WaitSema(s32 sema);
extern s32 WakeupThread(s32 id);
extern s32 RotateThreadReadyQueue(s32 priority);
extern s32 SuspendThread(s32 id);
extern s32 kprintf(const char *fmt, ...);

void topThread(TopThreadQueue *queue) {
    enum {
        THREAD_COMMAND_WAKEUP = 0,
        THREAD_COMMAND_ROTATE_READY_QUEUE = 1,
        THREAD_COMMAND_SUSPEND = 2
    };
    s32 i;
    u8  type;

    for (;;) {
        WaitSema(top_thread_semaphore);
        i = queue->read_index & 0x1FF;
        queue->read_index = i + 1;
        type = queue->requests[i].command;
        switch (type) {
        case THREAD_COMMAND_WAKEUP:
            WakeupThread(queue->requests[i].argument);
            break;
        case THREAD_COMMAND_ROTATE_READY_QUEUE:
            RotateThreadReadyQueue(queue->requests[i].argument);
            break;
        case THREAD_COMMAND_SUSPEND:
            SuspendThread(queue->requests[i].argument);
            break;
        default:
            kprintf((const char *)thread_command_error_message);
            break;
        }
    }
}
