#include "types.h"
#include "kernel.h"
#include "sda.h"

extern void *EndOfHeap(void);
extern int  *__errno(void) __asm__("func_001138A8");
/* Current program break, raised by sbrk. */
extern char *heap_break __asm__("D_0012FBF4") NOT_SDA;

/* Sony libkernel sbrk: moves the program break by increment with interrupts
   disabled; fails with ENOMEM past the end of the heap. */
void *sbrk(s32 increment) {
    u32   status = CpuReadStatus();
    char *proposed_heap_break;
    void *previous;

    status &= 0x10000;
    if (status) {
        do {
            CpuDisableInt();
            CpuSyncP();
        } while (CpuReadStatus() & 0x10000);
    }
    proposed_heap_break = heap_break + increment;
    if ((char *)EndOfHeap() < proposed_heap_break) {
        *__errno() = 12;
        if (status) {
            CpuEnableInt();
        }
        return (void *)-1;
    }
    previous = heap_break;
    heap_break = proposed_heap_break;
    if (status) {
        CpuEnableInt();
    }
    return previous;
}
