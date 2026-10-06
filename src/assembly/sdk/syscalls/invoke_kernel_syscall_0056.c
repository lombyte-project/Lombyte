#include "asm.h"

/* Exact Sony EE kernel syscall veneer 0056; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_0056/"
            "InvokeKernelSyscall0056.s",
            InvokeKernelSyscall0056);
