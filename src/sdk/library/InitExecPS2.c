/* InitExecPS2 i8: apply the kernel syscall patches when needed.
 * Hypothesis: declaring the return-ignored patch installer void frees v0
 * for the loop-test sltiu (retail v0; i5 ties at v1). */

#include "types.h"

extern s32 PatchIsNeeded();
extern void Rfu116SetSyscallLate();
extern s32 Rfu091Mid();
extern void InvokeKernelSyscall005A_RfuWorker(void *addr, void *table, s32 size);
extern void FlushCache(s32 mode);

typedef struct PatchPair {
    u32 a;
    u32 b;
} PatchPair;

extern PatchPair D_00130AD0[];
extern u8 exec_ps2_patch_data[] __asm__("D_00130328");

void InitExecPS2(void) {
    u32 i;

    if (PatchIsNeeded()) {
        Rfu116SetSyscallLate(D_00130AD0[0].a, D_00130AD0[0].b);
        InvokeKernelSyscall005A_RfuWorker((void *)0x80074000, exec_ps2_patch_data, 0x7A8);
        FlushCache(0);
        FlushCache(2);
        Rfu116SetSyscallLate(D_00130AD0[1].a, D_00130AD0[1].b);
        for (i = 2; i < 3; i++) {
            Rfu116SetSyscallLate(D_00130AD0[i].a, Rfu091Mid(D_00130AD0[i].a));
        }
    }
}
