#include "types.h"

extern u8   tlb_init_syscall_payload[] __asm__("D_0012FCB8");
extern s32  saved_syscall_3_handler[] __asm__("D_00130030");
extern s32  tlb_syscall_registration_pairs[] __asm__("D_00130038");
extern void FlushCache(s32 mode);
extern void InvokeKernelSyscall005A_Rfu(u32 addr, void *buf, s32 size);
extern s32  Rfu091Early(s32 arg);
extern void Rfu116SetSyscallEarly(s32 a, s32 b);

void InitTLBFunctions(void) {
    s32 *base;
    s32 *cursor;
    u32  i;

    base = tlb_syscall_registration_pairs;
    i = 3;
    Rfu116SetSyscallEarly(base[0], base[1]);
    InvokeKernelSyscall005A_Rfu(0x80075000, tlb_init_syscall_payload, 0x330);
    FlushCache(0);
    FlushCache(2);
    Rfu116SetSyscallEarly(base[2], base[3]);
    Rfu116SetSyscallEarly(base[4], base[5]);
    cursor = base + 6;
    do {
        Rfu116SetSyscallEarly(cursor[0], Rfu091Early(cursor[0]));
        cursor += 2;
        i += 1;
    } while (i < 8U);
    saved_syscall_3_handler[0] = Rfu091Early(3);
}
