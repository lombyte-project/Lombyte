#include "asm.h"

/* Exact Sony EE kernel syscall veneer ffda; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_ffda/"
            "InvokeKernelSyscallFFDA.s",
            InvokeKernelSyscallFFDA);
