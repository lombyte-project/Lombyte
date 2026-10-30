/* InitAlarm a11: apply the timer/syscall patches when needed.
 * Hypothesis: declaring the return-ignored patch installer void frees v0
 * for the loop-test sltiu (retail v0; a2 ties at v1). */

#include "types.h"

extern void Rfu116SetSyscallTail();
extern s32  Rfu091Tail();
extern void InvokeKernelSyscall005A_RfuTail(void *addr, void *table, s32 size);
extern void FlushCache(s32 mode);

typedef struct PatchPair {
    u32 a;
    u32 b;
} PatchPair;

extern PatchPair alarm_syscall_patch_pairs[] __asm__("D_00131258");
extern u8        alarm_kernel_patch_payload[] __asm__("D_00130AF0");
extern u8        alarm_call_trampoline_payload[] __asm__("D_00131230");

void InitAlarm(void) {
    u32 i;

    if ((*(volatile u32 *)0x10001810 & 0x100) == 0) {
        Rfu116SetSyscallTail(alarm_syscall_patch_pairs[0].a, alarm_syscall_patch_pairs[0].b);
        InvokeKernelSyscall005A_RfuTail((void *)0x80076000, alarm_kernel_patch_payload, 0x740);
        InvokeKernelSyscall005A_RfuTail((void *)0x00082000, alarm_call_trampoline_payload, 0x28);
        FlushCache(0);
        FlushCache(2);
        Rfu116SetSyscallTail(alarm_syscall_patch_pairs[1].a, alarm_syscall_patch_pairs[1].b);
        for (i = 2; i < 8; i++) {
            Rfu116SetSyscallTail(alarm_syscall_patch_pairs[i].a,
                                 Rfu091Tail(alarm_syscall_patch_pairs[i].a));
        }
    }
}
