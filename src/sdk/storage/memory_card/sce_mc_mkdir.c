#include "sda.h"
#include "rnc/sdk/libmc.h"
#include "rnc/sdk/storage/memory_card.h"

s32 sceMcMkdir(s32 port, s32 slot, const char *name) {
    enum {
        MC_MKDIR_REQUEST_ACCEPTED = 0
    };
    s32 mkdir_request_status = sceMcOpen(port, slot, name, 0x40);

    if (mkdir_request_status == MC_MKDIR_REQUEST_ACCEPTED) {
        mc_current_func = MC_FUNC_MKDIR;
    }
    return mkdir_request_status;
}
