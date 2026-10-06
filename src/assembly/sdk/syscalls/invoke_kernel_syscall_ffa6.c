#include "asm.h"

/* Exact Sony EE kernel syscall veneer ffa6; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_ffa6/"
            "InvokeKernelSyscallFFA6.s",
            InvokeKernelSyscallFFA6);
