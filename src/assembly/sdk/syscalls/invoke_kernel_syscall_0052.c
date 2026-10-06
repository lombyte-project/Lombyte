#include "asm.h"

/* Exact Sony EE kernel syscall veneer 0052; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_0052/"
            "InvokeKernelSyscall0052.s",
            InvokeKernelSyscall0052);
