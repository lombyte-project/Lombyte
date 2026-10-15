#ifndef LOMBYTE_RNC_SDK_TOP_THREAD_H
#define LOMBYTE_RNC_SDK_TOP_THREAD_H

#include "types.h"

/* Sony libkernel helper thread (InitThread, topThread): it runs the thread
   calls an interrupt handler cannot make itself, such as waking the thread
   it interrupted. */

/* One queued call: what to do (0 wake up, 1 rotate the ready queue,
   2 suspend) and the thread or priority it applies to. */
typedef struct TopThreadRequest {
    u8 command;
    u8 argument;
} TopThreadRequest;

typedef struct TopThreadQueue {
    s32 read_index;
    s32 write_index;
    TopThreadRequest requests[512];
} TopThreadQueue;

extern s32 top_thread_id __asm__("D_0012FBF8") __attribute__((section(".data")));
extern s32 top_thread_semaphore __asm__("D_00154500") __attribute__((section(".data")));
extern TopThreadQueue top_thread_queue __asm__("D_00154508");

#endif /* LOMBYTE_RNC_SDK_TOP_THREAD_H */
