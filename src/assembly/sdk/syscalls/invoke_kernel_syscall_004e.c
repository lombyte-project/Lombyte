#include "asm.h"

/* Exact Sony EE kernel syscall veneer 004e; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_004e/"
            "InvokeKernelSyscall004E.s",
            InvokeKernelSyscall004E);
