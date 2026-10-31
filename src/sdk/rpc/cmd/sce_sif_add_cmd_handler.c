#include "types.h"
#include "rnc/data_labels.h"

void sceSifAddCmdHandler(s32 command_id_or_handler_table_base, u32 handler, u32 handler_argument) {
    u32 handler_entry_address = command_id_or_handler_table_base << 3;

    if (command_id_or_handler_table_base < 0) {
        command_id_or_handler_table_base = D_00154E64[0];
    } else {
        command_id_or_handler_table_base = D_00154E6C[0];
    }
    handler_entry_address += command_id_or_handler_table_base;
    *(u32 *)handler_entry_address = handler;
    *(u32 *)(handler_entry_address + 4) = handler_argument;
}
