#include "rnc/sdk/kernel/init_tlb.h"
#include "rnc/sdk/syscalls/jump_to_rfu_dispatch.h"

extern void RfuDispatchEntry(int status);
extern void _Exit(int status);

void RfuDispatchEntry(int status) {
    JumpToRfuDispatch();
    _Exit(status);
}
