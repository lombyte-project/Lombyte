#include "asm.h"

/* Exact Sony EE kernel syscall veneer 0072; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_0072/"
            "InvokeKernelSyscall0072.s",
            InvokeKernelSyscall0072);
