#include "types.h"
#include "rnc/sdk/kernel.h"
#include "sda.h"
#include "rnc/sdk/libc/stdlib.h"

extern void *EndOfHeap(void);
/* Current program break, raised by sbrk. */
extern char *heap_break __asm__("D_0012FBF4") NOT_SDA;

/* Sony libkernel sbrk: moves the program break by increment with interrupts
   disabled; fails with ENOMEM past the end of the heap. */
void *sbrk(s32 increment) {
    u32   interrupts_were_enabled = CpuReadStatus();
    char *proposed_heap_break;
    void *previous;

    interrupts_were_enabled &= 0x10000;
    if (interrupts_were_enabled) {
        do {
            CpuDisableInt();
            CpuSyncP();
        } while (CpuReadStatus() & 0x10000);
    }
    proposed_heap_break = heap_break + increment;
    if ((char *)EndOfHeap() < proposed_heap_break) {
        *__errno() = 12;
        if (interrupts_were_enabled) {
            CpuEnableInt();
        }
        return (void *)-1;
    }
    previous = heap_break;
    heap_break = proposed_heap_break;
    if (interrupts_were_enabled) {
        CpuEnableInt();
    }
    return previous;
}
