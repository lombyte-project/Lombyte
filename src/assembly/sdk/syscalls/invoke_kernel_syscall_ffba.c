#include "asm.h"

/* Exact Sony EE kernel syscall veneer ffba; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_ffba/"
            "InvokeKernelSyscallFFBA.s",
            InvokeKernelSyscallFFBA);
