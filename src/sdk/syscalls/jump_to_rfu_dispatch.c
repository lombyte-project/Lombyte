#include "rnc/sdk/kernel.h"
#include "rnc/sdk/syscalls.h"

extern void JumpToRfuDispatch(void);
extern void _Exit(int status);

void JumpToRfuDispatch(void) {
    InitTLB();
}
