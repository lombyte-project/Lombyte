#include "asm.h"

/* Exact Sony EE kernel syscall veneer 005f; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_005f/"
            "InvokeKernelSyscall005F.s",
            InvokeKernelSyscall005F);
