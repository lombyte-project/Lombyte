#include "rnc/sdk/rpc/iop.h"
/* sceSifLoadModuleBuffer: load an IRX image already in IOP memory and return
   its module id (_sceSifLoadModuleBuffer also reports the start result). */

#include "types.h"

s32 sceSifLoadModuleBuffer(s32         iop_address,
                           s32         arguments_length,
                           const char *arguments) __asm__("InvokeIopStatus");

s32 sceSifLoadModuleBuffer(s32 iop_address, s32 arguments_length, const char *arguments) {
    s32 discarded_load_result;

    return _sceSifLoadModuleBuffer(
        iop_address, arguments_length, arguments, &discarded_load_result);
}
