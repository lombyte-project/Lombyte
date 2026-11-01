#include "rnc/sdk/kernel.h"
#include "rnc/sdk/syscalls.h"

extern void RfuDispatchEntry(int status);
extern void _Exit(int status);

void RfuDispatchEntry(int status) {
    JumpToRfuDispatch();
    _Exit(status);
}
