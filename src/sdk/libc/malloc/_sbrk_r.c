#include "sda.h"
/*
 * Derived from newlib, which is distributed under permissive BSD-style
 * terms. See THIRD_PARTY_NOTICES.md and licenses/COPYING.NEWLIB.txt.
 */

/* Source: newlib. */

extern int   SbrkErrno __asm__("D_0015EC10") NOT_SDA;
extern void *Sbrk(int increment) __asm__("sbrk");

void *SbrkReentrant(void *reent, int increment) __asm__("_sbrk_r");

void *SbrkReentrant(void *reent, int increment) {
    void *previous_heap_break;

    SbrkErrno = 0;
    previous_heap_break = Sbrk(increment);
    if (previous_heap_break == (void *)-1 && SbrkErrno != 0) {
        *(int *)reent = SbrkErrno;
    }
    return previous_heap_break;
}
