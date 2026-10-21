#include "types.h"
#include "rnc/data_labels.h"

void sceSifAddCmdHandler(s32 idx, u32 handler, u32 data) {
    u32 handler_entry_address = idx << 3;

    if (idx < 0) {
        idx = D_00154E64[0];
    } else {
        idx = D_00154E6C[0];
    }
    handler_entry_address += idx;
    *(u32 *)handler_entry_address = handler;
    *(u32 *)(handler_entry_address + 4) = data;
}
