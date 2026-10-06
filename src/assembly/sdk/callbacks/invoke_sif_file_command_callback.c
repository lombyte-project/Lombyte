#include "types.h"
#include "asm.h"

#ifndef NON_MATCHING
INCLUDE_ASM(
    "config/us/expected/asm/assembly/sdk/callbacks/invoke_sif_file_command_callback/FUN_0011b980.s",
    FUN_0011b980);
#else
typedef void (*SifFileCommandCallback)(void *);

typedef struct SifFileCommandCallbackRecord {
    SifFileCommandCallback callback;
    void *argument;
} SifFileCommandCallbackRecord;

/* The retail tail uses literal sync and ei instructions. The historical
 * compiler lowers these C operations to calls, so the assembly remains the
 * matching oracle. */
extern void __sync_synchronize(void);
extern void CpuEnableInt(void);

void FUN_0011b980(void *packet, SifFileCommandCallbackRecord *callback_record) {
    SifFileCommandCallback callback = callback_record->callback;

    if (callback) {
        callback(callback_record->argument);
    }
    __sync_synchronize();
    CpuEnableInt();
}
#endif /* NON_MATCHING */
