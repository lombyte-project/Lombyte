#include "rnc/sdk/kernel/init_tlb.h"
#include "rnc/sdk/syscalls/rfu_dispatch_entry.h"

extern void JumpToRfuDispatch(void);
extern void _Exit(int status);

void JumpToRfuDispatch(void) {
    InitTLB();
}
