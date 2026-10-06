#include "asm.h"

/* Exact Sony EE kernel syscall veneer ff02; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_ff02/"
            "InvokeKernelSyscallFF02.s",
            InvokeKernelSyscallFF02);
