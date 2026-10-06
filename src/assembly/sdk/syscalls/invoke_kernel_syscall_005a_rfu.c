#include "asm.h"

/* Exact RFU kernel syscall veneer (syscall 0x5A). */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_005a_rfu/"
            "InvokeKernelSyscall005A_Rfu.s",
            InvokeKernelSyscall005A_Rfu);
