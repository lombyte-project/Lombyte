#include "asm.h"

/* Exact Sony EE kernel syscall veneer 006c; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_006c/"
            "InvokeKernelSyscall006C.s",
            InvokeKernelSyscall006C);
