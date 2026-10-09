#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM("config/us/expected/asm/assembly/sdk/library/cmd_sem_init/cmd_sem_init.s",
            cmd_sem_init);
#else
#include "types.h"
#include "kernel.h"
extern volatile s32 D_001312E0[];
extern volatile s32 D_001312E8[];
extern s32 D_001312EC[];
extern volatile s32 D_001312F0[];
extern s32 CreateSema(struct SemaParam *);
void cmd_sem_init(void) {
    struct SemaParam sema;

    if ((D_001312E8[0] != -1) && (D_001312EC[0] != -1)) {
        return;
    }
    sema.option = 0;
    sema.initCount = 1;
    sema.maxCount = 1;
    D_001312E8[0] = CreateSema(&sema);
    D_001312EC[0] = CreateSema(&sema);
    sema.initCount = 0;
    D_001312E0[0] = CreateSema(&sema);
    D_001312F0[0] = 0;
}

#endif /* NON_MATCHING */
