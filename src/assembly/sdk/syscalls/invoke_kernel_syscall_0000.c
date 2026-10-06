#include "asm.h"

/* Exact Sony EE kernel syscall veneer 0000; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_0000/"
            "InvokeKernelSyscall0000.s",
            InvokeKernelSyscall0000);
