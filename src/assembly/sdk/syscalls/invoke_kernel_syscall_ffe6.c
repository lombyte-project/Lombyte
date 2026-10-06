#include "asm.h"

/* Exact Sony EE kernel syscall veneer ffe6; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_ffe6/"
            "InvokeKernelSyscallFFE6.s",
            InvokeKernelSyscallFFE6);
