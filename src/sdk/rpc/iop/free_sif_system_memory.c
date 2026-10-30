#include "types.h"
#include "rnc/data_labels.h"

extern s32 sceSifCallRpc();

s32 free_sif_system_memory(void *addr) __asm__("FUN_0011c938");

s32 free_sif_system_memory(void *addr) {
    if (D_0012FCAC[0] < 0) {
        return 0;
    }
    D_001580C0[0] = (u32)addr;
    if (sceSifCallRpc(D_00158040, 2, 0, D_001580C0, 4, D_00158080, 4, 0, 0) < 0) {
        return -1;
    }
    return D_00158080[0];
}

extern s32 func_0011C938(void *addr) __attribute__((alias("FUN_0011c938")));
