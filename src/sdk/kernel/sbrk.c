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
    char *next;
    void *previous;

    status &= 0x10000;
    if (status) {
        do {
            CpuDisableInt();
            CpuSyncP();
        } while (CpuReadStatus() & 0x10000);
    }
    next = heap_break + increment;
    if ((char *)EndOfHeap() < next) {
        *__errno() = 12;
        if (status) {
            CpuEnableInt();
        }
        return (void *)-1;
    }
    previous = heap_break;
    heap_break = next;
    if (status) {
        CpuEnableInt();
    }
    return previous;
}
