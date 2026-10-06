#include "asm.h"

/* Exact RFU kernel syscall veneer (syscall 0x83). */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_0083_rfu/"
            "InvokeKernelSyscall0083_Rfu.s",
            InvokeKernelSyscall0083_Rfu);
