#include "types.h"
#include "kernel.h"

typedef void (*SifFileCommandCallback)(void *);

typedef struct SifFileCommandCallbackRecord {
    SifFileCommandCallback callback;
    void *argument;
} SifFileCommandCallbackRecord;

void invoke_sif_file_command_callback(void *packet, SifFileCommandCallbackRecord *callback_record)
    __asm__("FUN_0011b980");

void invoke_sif_file_command_callback(void *packet, SifFileCommandCallbackRecord *callback_record) {
    SifFileCommandCallback callback = callback_record->callback;

    if (callback) {
        callback(callback_record->argument);
    }
    CpuSync();
    CpuEnableInt();
}

extern __typeof__(invoke_sif_file_command_callback) func_0011B980 __attribute__((alias("FUN_0011b980")));
