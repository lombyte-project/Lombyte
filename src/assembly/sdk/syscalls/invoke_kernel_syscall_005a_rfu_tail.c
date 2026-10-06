#include "asm.h"

/* Exact RFU tail kernel syscall veneer (syscall 0x5A). */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_005a_rfu_tail/"
            "InvokeKernelSyscall005A_RfuTail.s",
            InvokeKernelSyscall005A_RfuTail);
