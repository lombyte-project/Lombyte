#include "asm.h"

/* Exact Sony EE kernel syscall veneer ffd2; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_ffd2/"
            "InvokeKernelSyscallFFD2.s",
            InvokeKernelSyscallFFD2);
