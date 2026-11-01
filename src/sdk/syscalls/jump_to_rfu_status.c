#include "rnc/sdk/kernel.h"
#include "rnc/sdk/syscalls.h"

extern void _Exit(int status);

void JumpToRfuStatus(void) {
    RfuDispatchEntry(0);
}
