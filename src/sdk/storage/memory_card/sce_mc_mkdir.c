#include "sda.h"
#include "rnc/sdk/libmc.h"

extern s32 sceMcOpen(s32 port, s32 slot, const char *name, s32 flags);

s32 sceMcMkdir(s32 port, s32 slot, const char *name) {
    s32 mkdir_request_status = sceMcOpen(port, slot, name, 0x40);

    if (mkdir_request_status == 0) {
        mc_current_func = MC_FUNC_MKDIR;
    }
    return mkdir_request_status;
}
