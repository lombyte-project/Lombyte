#include "types.h"
#include "rnc/data_labels.h"

void sceSifRemoveCmdHandler(s32 idx) {
    u32 handler_entry_address = idx << 3;

    if (idx < 0) {
        idx = D_00154E64[0];
    } else {
        idx = D_00154E6C[0];
    }
    handler_entry_address += idx;
    *(u32 *)handler_entry_address = 0;
}
