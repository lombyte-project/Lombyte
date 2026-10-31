#include "rnc/sdk/kernel/init_tlb.h"
#include "rnc/sdk/syscalls/jump_to_rfu_dispatch.h"
#include "rnc/sdk/syscalls/rfu_dispatch_entry.h"

extern void _Exit(int status);

void JumpToRfuStatus(void) {
    RfuDispatchEntry(0);
}
