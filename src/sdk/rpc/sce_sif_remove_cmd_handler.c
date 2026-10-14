#include "types.h"
#include "rnc/data_labels.h"

void sceSifRemoveCmdHandler(s32 idx) {
    u32 off = idx << 3;

    if (idx < 0) {
        idx = D_00154E64[0];
    } else {
        idx = D_00154E6C[0];
    }
    off += idx;
    *(u32 *)off = 0;
}
