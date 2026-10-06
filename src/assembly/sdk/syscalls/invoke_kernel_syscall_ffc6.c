#include "asm.h"

/* Exact Sony EE kernel syscall veneer ffc6; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_ffc6/"
            "InvokeKernelSyscallFFC6.s",
            InvokeKernelSyscallFFC6);
