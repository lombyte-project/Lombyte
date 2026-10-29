#include "types.h"
#include "rnc/data_labels.h"

void sceSifRemoveCmdHandler(s32 command_id) {
    u32 handler_entry_address = command_id << 3;

    if (command_id < 0) {
        command_id = D_00154E64[0];
    } else {
        command_id = D_00154E6C[0];
    }
    handler_entry_address += command_id;
    *(u32 *)handler_entry_address = 0;
}
