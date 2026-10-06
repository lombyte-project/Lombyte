#include "asm.h"

/* Exact Sony EE kernel syscall veneer 007b; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_007b/"
            "InvokeKernelSyscall007B.s",
            InvokeKernelSyscall007B);
