#include "asm.h"

/* Exact Sony EE kernel syscall veneer ffca; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_ffca/"
            "InvokeKernelSyscallFFCA.s",
            InvokeKernelSyscallFFCA);
