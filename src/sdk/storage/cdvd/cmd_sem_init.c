#include "asm.h"

#include "types.h"
#include "kernel.h"
#include "rnc/sdk/libcdvd.h"

extern s32 CreateSema(struct SemaParam *);

/* Retail keeps these two in registers here, as plain (not volatile) words. */
#define NCMD_SEMA (*(s32 *)&cd_ncmd_sema)
#define SCMD_SEMA (*(s32 *)&cd_scmd_sema)

void cmd_sem_init(void) {
    struct SemaParam sema;

    if ((NCMD_SEMA != -1) && (SCMD_SEMA != -1)) {
        return;
    }
    sema.option = 0;
    sema.initCount = 1;
    sema.maxCount = 1;
    /* Both results use the same record; store the first ID before the second call. */
    SCMD_SEMA = CreateSema((NCMD_SEMA = CreateSema(&sema)) ? &sema : &sema);
    sema.initCount = 0;
    cd_callback_sema = CreateSema(&sema);
    cd_read_pending = 0;
}
