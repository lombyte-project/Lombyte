/* sceSifLoadModuleBuffer: load an IRX image already in IOP memory and return
   its module id (_sceSifLoadModuleBuffer also reports the start result). */

#include "types.h"

extern s32 _sceSifLoadModuleBuffer(s32         module_id,
                                   s32         argument_count,
                                   const char *arguments,
                                   void       *result) __asm__("_sceSifLoadModuleBuffer");

s32 sceSifLoadModuleBuffer(s32         iop_address,
                           s32         arguments_length,
                           const char *arguments) __asm__("InvokeIopStatus");

s32 sceSifLoadModuleBuffer(s32 iop_address, s32 arguments_length, const char *arguments) {
    s32 result;

    return _sceSifLoadModuleBuffer(iop_address, arguments_length, arguments, &result);
}
