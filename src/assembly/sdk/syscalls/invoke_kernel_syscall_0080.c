#include "asm.h"

/* Exact kernel syscall veneer (syscall 0x80). */

INCLUDE_ASM("config/us/expected/asm/assembly/sdk/syscalls/invoke_kernel_syscall_0080/"
            "InvokeKernelSyscall0080.s",
            InvokeKernelSyscall0080);
