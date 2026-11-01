#include "sda.h"
#include "rnc/sdk/libmc.h"
#include "rnc/sdk/storage/memory_card/sce_mc_open.h"

s32 sceMcMkdir(s32 port, s32 slot, const char *name) {
    s32 mkdir_request_status = sceMcOpen(port, slot, name, 0x40);

    if (mkdir_request_status == 0) {
        mc_current_func = MC_FUNC_MKDIR;
    }
    return mkdir_request_status;
}
