#include "asm.h"

/* Exact Sony EE kernel syscall veneer 005d; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_005d/"
            "InvokeKernelSyscall005D.s",
            InvokeKernelSyscall005D);
