#include "asm.h"

/* Exact Sony EE kernel syscall veneer 000c; no C wrapper preserves the ABI body. */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_000c/"
            "InvokeKernelSyscall000C.s",
            InvokeKernelSyscall000C);
