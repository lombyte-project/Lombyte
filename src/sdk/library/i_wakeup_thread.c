#include "types.h"
#include "kernel.h"
#include "rnc/sdk/top_thread.h"

extern s32 _iWakeupThread(s32 thread_id);
extern s32 iSignalSema(s32 semaphore_id);

/* Sony libkernel iWakeupThread: a thread cannot wake itself through the
   kernel, so that case is queued for the top thread instead. */
s32 iWakeupThread(s32 thread_id) {
    s32 current = CpuSyscall(-47); /* iGetThreadId */
    u32 index;

    if (current != thread_id) {
        return _iWakeupThread(thread_id);
    }
    if ((u32)current >= 256 || !top_thread_id) {
        return -1;
    }
    index = top_thread_queue.write_index & 0x1ff;
    top_thread_queue.write_index = index + 1;
    top_thread_queue.requests[index].command = 0;
    top_thread_queue.requests[index].argument = current;
    iSignalSema(top_thread_semaphore);
    return current;
}
